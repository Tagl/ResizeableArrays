#pragma once

#include <stdexcept>
#include <stdint.h>

namespace {
	constexpr uint64_t num_large_blocks(uint64_t B)
	{
		return B;
	}

	constexpr uint64_t large_block_size(uint64_t B) noexcept
	{
		return B * B;
	}

	constexpr uint64_t num_small_blocks(uint64_t B) noexcept
	{
		return 2 * B;
	}

	constexpr uint64_t small_block_size(uint64_t B) noexcept
	{
		return B;
	}
};

template <typename T>
class TarjanZwick
{
public:
	TarjanZwick();
	TarjanZwick(const TarjanZwick& other);
	~TarjanZwick();
	void push_back(const T& val);
	void pop_back();
	T& at(uint64_t pos);
	T& operator[](uint64_t pos);
	uint64_t potential_capacity() const noexcept;
	uint64_t capacity() const noexcept;
	uint64_t size() const noexcept;

private:
	void rebuild_helper(T**& data, uint64_t& ind, uint64_t& nextCapacity, uint64_t numBlocks, uint64_t blockSize, uint64_t oldNumBlocks, uint64_t oldBlockSize);
	void rebuild(uint64_t nextB);
	void grow();
	void shrink();

	T** large;
	T** small;
	uint64_t B = MIN_B;
	uint64_t _capacity = 0;
	uint64_t _size = 0;
	static const uint64_t MIN_B = 1;
	// N^(1/3) <= B <= 4*N^(1/3) ==> N <= B^3 <= 64*N
	static const uint64_t DENOMINATOR = 64;
};

template<typename T>
inline TarjanZwick<T>::TarjanZwick()
{
	large = new T*[num_large_blocks(B)];
	for (uint64_t i = 0; i < num_large_blocks(B); i++)
	{
		large[i] = nullptr;
	}
	small = new T*[num_small_blocks(B)];
	for (uint64_t i = 0; i < num_small_blocks(B); i++)
	{
		small[i] = nullptr;
	}
}

template<typename T>
inline TarjanZwick<T>::TarjanZwick(const TarjanZwick& other) : TarjanZwick()
{
	rebuild(other.B);
	for (uint64_t i = 0; i < other.size(); i++)
	{
		push_back(other[i]);
	}
}

template<typename T>
inline TarjanZwick<T>::~TarjanZwick()
{
	for (uint64_t i = 0; i < num_large_blocks(B); i++)
	{
		if (large[i] != nullptr)
		{
			delete[] large[i];
		}
	}
	delete[] large;

	for (uint64_t i = 0; i < num_small_blocks(B); i++)
	{
		if (small[i] != nullptr)
		{
			delete[] small[i];
		}
	}
	delete[] small;
}

template<typename T>
inline void TarjanZwick<T>::push_back(const T& val)
{
	if (_size == potential_capacity())
	{
		rebuild(B * 2);
	}
	if (_size == _capacity)
	{
		grow();
	}
	(*this)[_size++] = val;
}

template<typename T>
inline void TarjanZwick<T>::pop_back()
{
	_size--;
	if (B > MIN_B && DENOMINATOR * _size < num_large_blocks(B) * large_block_size(B))
	{
		rebuild(B / 2);
	}
	bool shouldShrink = false;
	if (_capacity <= num_large_blocks(B) * large_block_size(B))
	{
		shouldShrink = _size + 2 * large_block_size(B) <= _capacity;
	}
	else if (_capacity <= num_large_blocks(B) * large_block_size(B) + small_block_size(B)) 
	{
		shouldShrink = _size + large_block_size(B) + small_block_size(B) <= _capacity;
	}
	else
	{
		shouldShrink = _size + 2 * small_block_size(B) <= _capacity;
	}
	if (shouldShrink)
	{
		shrink();
	}
}

template<typename T>
inline T& TarjanZwick<T>::at(uint64_t pos)
{
	if (pos < 0 || size() <= pos) throw std::out_of_range();
	return this[pos];
}

template<typename T>
inline T& TarjanZwick<T>::operator[](uint64_t pos)
{
	uint64_t B2 = large_block_size(B);
	uint64_t B3 = num_large_blocks(B) * large_block_size(B);
	return pos < B3 ? large[pos/B2][pos%B2] : small[(pos-B3)/B][pos%B];
}

template<typename T>
inline uint64_t TarjanZwick<T>::potential_capacity() const noexcept
{
	return num_large_blocks(B) * large_block_size(B) + num_small_blocks(B) * small_block_size(B);
}

template<typename T>
inline uint64_t TarjanZwick<T>::capacity() const noexcept
{
	return _capacity;
}

template<typename T>
inline uint64_t TarjanZwick<T>::size() const noexcept
{
	return _size;
}

template<typename T>
inline void TarjanZwick<T>::rebuild_helper(T**& data, uint64_t& ind, uint64_t& nextCapacity, uint64_t numBlocks, uint64_t blockSize, uint64_t oldNumBlocks, uint64_t oldBlockSize)
{
	T** next = new T*[numBlocks];
	uint64_t block = 0;
	for (uint64_t i = 0; i < numBlocks; i++)
	{
		if (ind < size())
		{
			next[i] = new T[blockSize];
			nextCapacity += blockSize;
			for (uint64_t j = 0; j < blockSize; j++)
			{
				if (ind < size())
				{
					next[i][j] = (*this)[ind];
				}
				ind++;
				if (block < B && data[block] != nullptr && ind % oldBlockSize == 0)
				{
					delete[] data[block];
					data[block] = nullptr;
					block++;
				}
			}
		}
		else
		{
			next[i] = nullptr;
			ind += blockSize;
		}
	}
	while (block < oldNumBlocks && data[block] != nullptr) {
		delete[] data[block++];
	}
	delete[] data;
	data = next;
}

template<typename T>
inline void TarjanZwick<T>::rebuild(uint64_t nextB)
{
	uint64_t nextCapacity = 0;
	uint64_t ind = 0;
	rebuild_helper(large, ind, nextCapacity, num_large_blocks(nextB), large_block_size(nextB), num_large_blocks(B), large_block_size(B));
	rebuild_helper(small, ind, nextCapacity, num_small_blocks(nextB), small_block_size(nextB), num_small_blocks(B), small_block_size(B));
	_capacity = nextCapacity;
	B = nextB;
}

template<typename T>
inline void TarjanZwick<T>::grow()
{
	uint64_t totalLargeSize = num_large_blocks(B) * large_block_size(B);
	if (_size < totalLargeSize)
	{
		large[_size / large_block_size(B)] = new T[large_block_size(B)];
		_capacity += large_block_size(B);
	}
	else
	{
		small[(_size - totalLargeSize) / small_block_size(B)] = new T[small_block_size(B)];
		_capacity += small_block_size(B);
	}
}

template<typename T>
inline void TarjanZwick<T>::shrink()
{
	uint32_t totalLargeSpace = num_large_blocks(B) * large_block_size(B);
	if (_capacity <= totalLargeSpace)
	{
		delete[] large[_capacity / large_block_size(B) - 1];
		large[_capacity / large_block_size(B) - 1] = nullptr;
		_capacity -= large_block_size(B);
	}
	else
	{
		delete[] small[(_capacity - totalLargeSpace) / small_block_size(B) - 1];
		small[(_capacity - totalLargeSpace) / small_block_size(B) - 1] = nullptr;
		_capacity -= small_block_size(B);
	}
}

