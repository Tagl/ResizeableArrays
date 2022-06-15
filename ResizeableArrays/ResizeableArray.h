#pragma once

#include <stdint.h>

template <typename T>
class ResizeableArray
{
public:
	ResizeableArray();
	ResizeableArray(const ResizeableArray& other);
	~ResizeableArray();
	void push_back(const T& val);
	void pop_back();
	T& at(uint64_t pos);
	T& operator[](uint64_t pos);
	uint64_t potential_capacity() const noexcept;
	uint64_t capacity() const noexcept;
	uint64_t size() const noexcept;
private:
	void rebuild(uint64_t nextCapacity);
	void grow();
	void shrink();

	T* data;
	uint64_t _capacity = 2;
	uint64_t _size = 0;
};

template<typename T>
inline ResizeableArray<T>::ResizeableArray()
{
	data = new T[_capacity];
}

template<typename T>
inline ResizeableArray<T>::ResizeableArray(const ResizeableArray& other)
{
	_capacity = other._capacity;
	_size = other._size;
	data = new T[_capacity];
	for (uint64_t i = 0; i < _size; i++)
	{
		data[i] = other.data[i];
	}
}

template<typename T>
inline ResizeableArray<T>::~ResizeableArray()
{
	delete[] data;
}

template<typename T>
inline void ResizeableArray<T>::push_back(const T& val)
{
	if(_size == potential_capacity()) grow();
	data[_size++] = val;
}

template<typename T>
inline void ResizeableArray<T>::pop_back()
{
	_size--;
	if (4 * _size <= potential_capacity()) shrink();
}

template<typename T>
inline T& ResizeableArray<T>::at(uint64_t pos)
{
	return this[pos];
}

template<typename T>
inline T& ResizeableArray<T>::operator[](uint64_t pos)
{
	return data[pos];
}

template<typename T>
inline uint64_t ResizeableArray<T>::potential_capacity() const noexcept
{
	return capacity();
}

template<typename T>
inline uint64_t ResizeableArray<T>::capacity() const noexcept
{
	return _capacity;
}

template<typename T>
inline uint64_t ResizeableArray<T>::size() const noexcept
{
	return _size;
}

template<typename T>
inline void ResizeableArray<T>::rebuild(uint64_t nextCapacity)
{
	T* store = new T[nextCapacity];
	for (uint64_t i = 0; i < _size; i++)
	{
		store[i] = data[i];
	}
	delete[] data;
	_capacity = nextCapacity;
	data = store;
}

template<typename T>
inline void ResizeableArray<T>::grow()
{
	rebuild(_capacity * 2);
}

template<typename T>
inline void ResizeableArray<T>::shrink()
{
	rebuild(_capacity / 2);
}
