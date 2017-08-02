//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	GambleKit Copyright (C) 2014 Ivan Donev
//
//	This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//	contact: gamblekit@gmail.com
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

#include "..\GkDefinitions.h"

#include "..\OSSpecific\GkOSUtils.h"

template <class GKDLLCLASS T> class GKDLLCLASS GkList
{
public:
	static const int ListIndex_NotFound = -1;

	int size, capacity;
	T *values;

	T& operator [](int index) 
	{ 
		return this->values[index]; 
	};

	GkList::GkList(int capacity = 8)
	{
		this->capacity = 0;
		this->size = 0;
		this->values = 0;

		this->Resize(capacity);
	};

	GkList::~GkList(void)
	{
		delete this->values;
	};

	void GkList::Resize(int newCapacity)
	{
		T *newValues = new T[newCapacity];
	
		if (this->size > 0)
			GkOSUtils::CopyMem(newValues, values, this->size * sizeof(T));
		if (this->values != 0)
			delete this->values;

		this->values = newValues;
		this->capacity = newCapacity;
	};

	T GkList::Add(T value)
	{
		if (this->size >= this->capacity)
			this->Resize(this->capacity * 2);

		this->values[this->size++] = value;

		return value;
	};

	T GkList::Insert(T value, int index)
	{
		if (this->size >= this->capacity)
			this->Resize(this->capacity * 2);

		for (int i = this->size; i > index; i--)
			this->values[i] = this->values[i - 1];

		this->values[index] = value;

		this->size++;
	};

	bool GkList::Remove(T value)
	{
		int index = this->IndexOf(value);
		if (index != GkList::ListIndex_NotFound)
		{
			this->RemoveIndex(index);
			return true;
		}

		return false;
	};

	void GkList::RemoveIndex(int index)
	{
		for (int i = index; i < this->size -1; i++)
			this[i] = this[i + 1];
		this->size--;
	};

	int GkList::IndexOf(T value)
	{
		for (int i = 0; i < this->size; i++)
			if (this->values[i] == value)
				return i;

		return GkList::ListIndex_NotFound;
	};
};

