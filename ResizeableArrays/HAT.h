#pragma once

#include <stdexcept>
#include <stdint.h>

template <typename T>
class HAT
{
public:
	HAT();
	HAT(const HAT& other);
	~HAT();
	void push_back(const T& val);
	void pop_back();
	T& at(uint64_t pos);
	T& operator[](uint64_t pos);
	uint64_t potential_capacity() const noexcept;
	uint64_t capacity() const noexcept;
	uint64_t size() const noexcept;
private:
	void rebuild(uint64_t nextB);
	void grow();
	void shrink();

	T** data;
	uint64_t B = MIN_B;
	uint64_t _capacity = 0;
	uint64_t _size = 0;
	static const uint64_t MIN_B = 1;
	// sqrt(N) <= B <= 4*sqrt(N) ==> N <= B^2 <= 16*N
	static const uint64_t DENOMINATOR = 16;
};

template<typename T>
inline HAT<T>::HAT()
{
	data = new T*[B];
	for (uint64_t i = 0; i < B; i++)
	{
		data[i] = nullptr;
	}
}

template<typename T>
inline HAT<T>::HAT(const HAT& other) : HAT()
{
	rebuild(other.B);
	for (uint64_t i = 0; i < other.size(); i++)
	{
		push_back(other[i]);
	}
}

template<typename T>
inline HAT<T>::~HAT()
{
	for (uint64_t i = 0; i < B; i++)
	{
		if (data[i] != nullptr)
		{
			delete[] data[i];
		}
	}
	delete[] data;
}

template<typename T>
inline void HAT<T>::push_back(const T& val)
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
inline void HAT<T>::pop_back()
{
	_size--;
	if (B > MIN_B && DENOMINATOR * _size < potential_capacity())
	{
		rebuild(B / 2);
	}
	if (_size + 2*B <= _capacity )
	{
		shrink();
	}
}

template<typename T>
inline T& HAT<T>::at(uint64_t pos)
{
	if (pos < 0 || size() <= pos) throw std::out_of_range();
	return this[pos];
}

template<typename T>
inline T& HAT<T>::operator[](uint64_t pos)
{
	return data[pos/B][pos%B];
}

template<typename T>
inline uint64_t HAT<T>::potential_capacity() const noexcept
{
	return B*B;
}

template<typename T>
inline uint64_t HAT<T>::capacity() const noexcept
{
	return _capacity;
}

template<typename T>
inline uint64_t HAT<T>::size() const noexcept
{
	return _size;
}

template<typename T>
inline void HAT<T>::rebuild(uint64_t nextB)
{
	T** store = new T*[nextB];
	uint64_t nextCapacity = 0;
	uint64_t bucket = 0;
	for (uint64_t i = 0, ind = 0; i < nextB; i++)
	{
		if (ind < size())
		{
			store[i] = new T[nextB];
			nextCapacity += nextB;
			for (uint64_t j = 0; j < nextB; j++)
			{
				if (ind < size()) store[i][j] = (*this)[ind];
				ind++;
				if (bucket < B && data[bucket] != nullptr && ind % B == 0)
				{
					delete[] data[bucket];
					data[bucket] = nullptr;
					bucket++;
				}
			}
		}
		else
		{
			store[i] = nullptr;
		}
	}
	while (bucket < B && data[bucket] != nullptr) {
		delete[] data[bucket++];
	}
	delete[] data;
	B = nextB;
	_capacity = nextCapacity;
	data = store;
}

template<typename T>
inline void HAT<T>::grow()
{
	data[_size / B] = new T[B];
	_capacity += B;
}

template<typename T>
inline void HAT<T>::shrink()
{
	delete[] data[_capacity / B - 1];
	data[_capacity / B - 1] = nullptr;
	_capacity -= B;
}
