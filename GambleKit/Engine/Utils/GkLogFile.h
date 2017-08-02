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

#include "GkStrings.h"

#include <fstream>

template <class GKDLLCLASS T> class GKDLLCLASS GkLogFile
{
public:
	wchar_t *fileName;
	int numRecords;

	GkLogFile(const wchar_t *fileName)
	{
		this->fileName = GkStrings::Copy(fileName);
		
		std::fstream file(this->fileName, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
		this->numRecords = (int)file.tellp() / (int)sizeof(T);
		file.close();
	};

	~GkLogFile(void)
	{
		if (this->fileName != 0)
			delete[] this->fileName;
	};

	void Write(T &record)
	{
		std::fstream file(this->fileName, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
		file.write((char*)&record, sizeof(T));
		file.close();

		this->numRecords++;
	};

	bool Read(int recordIndex, T &record)
	{
		if (recordIndex >= this->numRecords)
			return false;

		std::fstream file(this->fileName, std::ios::in | std::ios::binary);
		file.seekg(recordIndex * sizeof(T));
		file.read((char*)&record, sizeof(T));
		file.close();

		return true;
	};

	void Clear()
	{
		std::fstream file(this->fileName, std::ios::out | std::ios::trunc);
		file.close();

		this->numRecords = 0;
	};
};
