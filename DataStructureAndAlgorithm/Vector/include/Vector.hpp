#pragma once
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include"algorithm"
#include"assert.h"

template<typename Object>
class Vector
{
public:
	using iterator = Object*;						//迭代器类型	
	using pointer = Object * ;
	using reference = Object & ;

	Vector():_start(nullptr),_end(nullptr),_end_of_storage(nullptr),theSize(0),theCapacity(0){}

	Vector(int nSize, const Object& val)
	{
		assert(nSize >= 0);
		if (!nSize)
		{
			_start = nullptr;
			_end = nullptr;
			_end_of_storage = nullptr;
			theSize = 0;
			theCapacity = 0;
		}
		else
		{
			int SPARE_CAPACITY = 1;
			while (SPARE_CAPACITY < nSize)
				SPARE_CAPACITY *= 2;

			_start = new Object[SPARE_CAPACITY];
			_end = _start + nSize;
			_end_of_storage = _start + SPARE_CAPACITY;
			theSize = nSize;
			theCapacity = SPARE_CAPACITY;

			iterator iterator_begin = _start;
			while (nSize--)
				*iterator_begin++ = val;
		}
	}

	Vector(int nSize)
	{
		assert(nSize >= 0);
		if (!nSize)
		{
			_start = nullptr;
			_end = nullptr;
			_end_of_storage = nullptr;
			theSize = 0;
			theCapacity = 0;
		}
		else
		{
			int SPARE_CAPACITY = 1;
			while (SPARE_CAPACITY < nSize)
				SPARE_CAPACITY *= 2;

			_start = new Object[SPARE_CAPACITY];
			_end = _start + nSize;
			_end_of_storage = _start + SPARE_CAPACITY;
			theSize = nSize;
			theCapacity = SPARE_CAPACITY;

		}
	}

	Vector(const Vector& rhs) 
	{
		if (this = &rhs)
			return;
		else if (rhs.empty())
		{
			_start = nullptr;
			_end = nullptr;
			_end_of_storage = nullptr;
			theSize = 0;
			theCapacity = 0;
		}
		else
		{
			theSize = rhs.size();
			theCapacity = rhs.capcity();
			_start = new Object[theCapacity];
			_end = _start + theSize;
			_end_of_storage = _start + theCapacity;
			
			iterator this_begin = _start;
			iterator rhs_begin = rhs.begin();
			int nSize = theSize;
			while (nSize--)
				*this_begin++ = *rhs_begin++;
		}

	}

	/*Vector& operator=(const Vector& rhs)
	{
		theSize = rhs.size();
		theCapacity = rhs.capcity();
		objects = new Object[theCapacity];

		if (theSize > 0)
			for (int k = 0; k < theSize; ++k)
				*(objects + k) = rhs.at(k);

		return *this;
	}*/

	~Vector()
	{
		delete[] _start;
		_start = nullptr;
		_end = nullptr;
		_end_of_storage = nullptr;
		theSize = 0;
		theCapacity = 0;
	}

	reference front() { return *_start; }
	
	reference back() { return *(_end - 1); }

	iterator begin() const { return _start; }

	iterator end() const {return _end;}

	int size() const { return theSize; }

	bool empty() const { return size() == 0; }

	int capcity() const { return theCapacity; }

	void resize(int newSize, const Object T);

	void push_back(const Object& T)
	{
		insert(end(), 1, T);
	}

	void pop_back()
	{
		_end--;
		theSize--;
	}

	void insert(iterator it, const Object& x) {insert(it, 1, x);}

	void insert(iterator it, int n, const Object T);

	Object at(int pos) const
	{
		assert(pos >= 0 && pos < theSize);
		return *(_start + pos);
	}

	reference operator[](int pos)
	{
		assert(pos >= 0 && pos < theSize);
		return *(_start + pos);
	}

	void erase(iterator pos)                          //删除指定位置的元素
	{
		erase(pos, pos + 1);
	}

	void erase(iterator first, iterator end)             //删除范围元素  [_first,_end)
	{
		int len = end() - end;
		for (int index = 0; index < len; ++index)             //后边的元素前移
		{
			*(first + index) = *(end + index);
		}
		while (end() - first > len)
		{
			pop_back();
		}
	}

	void swap(Vector& other)                                   //交换数据
	{
		Vector tem = other;
		other = *this;
		*this = tem;
	}

	void assign(int n, const Object& x)                  //给容器中某一个位置赋值
	{
		*(_start + n - 1) = x;
	}
protected:
	iterator _start;
	iterator _end;
	iterator _end_of_storage;

	int theSize;
	int theCapacity;
};

template<typename Object>
void Vector<Object>::resize(int newSize, const Object T)
{
	assert(newSize >= 0);
	if (!newSize)
	{
		delete[] _start;
		_start = nullptr;
		_end = nullptr;
		_end_of_storage = nullptr;
		theSize = 0;
		theCapacity = 0;
	}
	else
	{
		delete[] _start;
		_start = nullptr;
		_end = nullptr;
		_end_of_storage = nullptr;
		theSize = 0;
		theCapacity = 0;

		int SPARE_CAPACITY = 1;
		while (SPARE_CAPACITY < newSize)
			SPARE_CAPACITY *= 2;

		_start = new Object[SPARE_CAPACITY];
		_end = _start + newSize;
		_end_of_storage = _start + SPARE_CAPACITY;
		theSize = newSize;
		theCapacity = SPARE_CAPACITY;

		iterator iterator_begin = _start;
		while (newSize--)
			*iterator_begin++ = T;
	}
}

template<typename Object>
void Vector<Object>::insert(iterator it, int n, const Object T)
{
	bool isExpansion = false; //默认不扩容
	int newSize = n + theSize;
	if (newSize > theCapacity)
		isExpansion = true;

	if (!isExpansion)
	{
		iterator old_end = _end;
		_end = _end + n;

		for (iterator i = old_end - 1; i >= it; --i)
			*(i + n) = *i;

		for (iterator i = it; i <= it + n - 1; ++i)
			*i = T;

		theSize += n;
		_end = _start + theSize;
	}
	else
	{
		int SPARE_CAPACITY = 1;
		while (SPARE_CAPACITY < newSize)
			SPARE_CAPACITY *= 2;

		Object* new_start = new Object[SPARE_CAPACITY];
		iterator tmp_iterator = _start;
		int pos = it - _start;

		//将it前面的Object复制到新vector
		for (iterator i = new_start; i <= new_start +pos-1; i++)
		{
			*i = *tmp_iterator++;
		}

		iterator old_end = _end;
		
		//将it后面源vector的Object向后移动n位
		for (iterator i = old_end - 1,j = new_start + newSize-1; i >= it; --i,--j)
			*j = *i;

		for (iterator i = new_start+pos; i <= new_start + pos + n - 1; ++i)
			*i = T;

		delete[] _start;
		theSize += n;
		_start = new_start;
		_end = _start + theSize;
		_end_of_storage = _start + SPARE_CAPACITY;
		theCapacity = SPARE_CAPACITY;
	}
}
#endif // !VECTOR_HPP
