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

#include "GkFileSystem.h"

#include <Windows.h>
#include <vector>
#include <string>
#include "..\Utils\GkStrings.h"

std::wstring GkFileSystem::source;
//wchar_t *GkFileSystem::source;
FileSystemMode GkFileSystem::mode;
GkFileSystem::Init GkFileSystem::init;

GkFileSystem::Init::Init(void)
{
	//GkFileSystem::source = GkStrings::Copy(L"\0");
}

GkFileSystem::Init::~Init(void)
{
	//delete[] GkFileSystem::source;
}

GkFileSystem::GkFileSystem(void)
{
}


GkFileSystem::~GkFileSystem(void)
{
}


void GkFileSystem::SetSource(const wchar_t *source, FileSystemMode mode)
{
	//delete[] GkFileSystem::source;

	//if (source == 0)
	//	GkFileSystem::source = GkStrings::Copy(L"\0");
	//else
	//	GkFileSystem::source = GkStrings::Copy(source);

	if (source == 0)
		GkFileSystem::source = std::wstring(L"\0");
	else
		GkFileSystem::source = std::wstring(source);



	GkFileSystem::mode = mode;
}

bool GkFileSystem::LoadFile(const wchar_t *fileName, void **fileData, unsigned int *fileSize)
{
	if (GkFileSystem::mode == FileSystemMode_Physical)
		return GkFileSystem::LoadPhysicalFile(fileName, fileData, fileSize);
	
	if (GkFileSystem::mode == FileSystemMode_Virtual)
		return GkFileSystem::LoadVirtualFile(fileName, fileData, fileSize);

	return false;
}

void GkFileSystem::CreateVirtualFileSystem(const wchar_t* directory, const wchar_t* fileName)
{
	std::ofstream fileStream(fileName, std::ios::out | std::ios::binary);

	GkFileSystem::WriteVirtualFileSystem(directory, &fileStream);

	fileStream.close();
}

bool GkFileSystem::LoadVirtualFile(const wchar_t *fileName, void **fileData, unsigned int *fileSize)
{
	std::vector<wchar_t*> pathElements;

	int elementStart = 0;
	int i = 0;
	bool done;
	do
	{
		done = *((char*)&fileName[i]) == 0;

		if ((fileName[i] == L'\\') || done)
		{
			int l = i - elementStart;
			wchar_t *element = new wchar_t[l + 1];
			CopyMemory(element, &fileName[elementStart], l * sizeof(wchar_t));
			element[l] = 0;
			pathElements.push_back(element);
			elementStart = i + 1;
		}

		i++;
	} while (!done);

	
	std::ifstream file(GkFileSystem::source, std::ios::in | std::ios::binary | std::ios::ate);
	int pathElementIndex = 0;

	int dirLimit = (int)file.tellg();
	file.seekg(0);

	done = false;
	while (file && !done)
	{
		wchar_t *vfn = GkFileSystem::ReadString(&file);
		int size;
		file.read((char*)&size, sizeof(int));

		if (wcscmp(vfn, pathElements[pathElementIndex]) == 0)
		{
			pathElementIndex++;
			dirLimit = (int)file.tellg() + size;

			if (pathElementIndex == pathElements.size())
			{
				*fileSize = size;
				*fileData = new char[size];
				file.read((char*)*fileData, size);

				done = true;
			}
		}
		else
		{
			file.seekg((int)file.tellg() + size);
		}

		delete vfn;

		if (file.tellg() >= dirLimit)
			break;
	}

	file.close();

	
	for (int i = 0; i < (int)pathElements.size(); i++)
	{
		delete pathElements[i];
	}

	return done;
}

bool GkFileSystem::LoadPhysicalFile(const wchar_t *fileName, void **fileData, unsigned int *fileSize)
{
	// TODO: fix this static string
	std::wstring fn(GkFileSystem::source);
	fn.append(fileName);

	std::ifstream file(fn.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

	if (!file)
		return false;

	*fileSize = (unsigned int)file.tellg();
	*fileData = new char[*fileSize];
	
	file.seekg(0);
	file.read((char*)*fileData, *fileSize);

	file.close();

	return true;
}

wchar_t* GkFileSystem::ReadString(std::ifstream* fileStream)
{
	int l;
	fileStream->read((char*)&l, sizeof(int));

	wchar_t *result = new wchar_t[l + 1];
	fileStream->read((char*)result, l * sizeof(wchar_t));
	result[l] = 0;

	return result;
}

void GkFileSystem::WriteVirtualFileSystem(const wchar_t* directory, std::ofstream* fileStream)
{
	std::wstring fullSearchDir(directory);
	fullSearchDir.append(L"\\*");
	
	WIN32_FIND_DATAW findData;

	HANDLE findHandle = INVALID_HANDLE_VALUE;
	findHandle = FindFirstFileW(fullSearchDir.c_str(), &findData);

	if (findHandle == INVALID_HANDLE_VALUE)
	{
		return;
	}

	int chunkSize, chunkSizePos;
	do
	{
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if ((wcscmp(findData.cFileName, L".") != 0) && (wcscmp(findData.cFileName, L"..") != 0))
			{
				std::wstring fullSearchSubDir(directory);
				fullSearchSubDir.append(L"\\");
				fullSearchSubDir.append(findData.cFileName);
			
				GkFileSystem::WriteString(findData.cFileName, fileStream);

				chunkSizePos = (int)fileStream->tellp();
				fileStream->write((char*)&chunkSize, sizeof(int));

				GkFileSystem::WriteVirtualFileSystem(fullSearchSubDir.c_str(), fileStream);

				int endPos = (int)fileStream->tellp();
				chunkSize = endPos - chunkSizePos - 4;
				fileStream->seekp(chunkSizePos);
				fileStream->write((char*)&chunkSize, sizeof(int));
				fileStream->seekp(endPos);
			}
		}
		else
		{
			std::wstring fullFileName(directory);
			fullFileName.append(L"\\");
			fullFileName.append(findData.cFileName);

			GkFileSystem::WriteString(findData.cFileName, fileStream);

			chunkSizePos = (int)fileStream->tellp();
			fileStream->write((char*)&chunkSize, sizeof(int));

			GkFileSystem::WriteFile(fullFileName.c_str(), fileStream);

			int endPos = (int)fileStream->tellp();
			chunkSize = endPos - chunkSizePos - 4;
			fileStream->seekp(chunkSizePos);
			fileStream->write((char*)&chunkSize, sizeof(int));
			fileStream->seekp(endPos);
		}

	}
	while (FindNextFileW(findHandle, &findData) != 0);
 
   FindClose(findHandle);
}

void GkFileSystem::WriteString(const wchar_t* s, std::ofstream* fileStream)
{
	int l = 0;
	while (s[l] != 0) 
		l++;

	fileStream->write((char*)&l, sizeof(int));
	fileStream->write((char*)s, l * sizeof(wchar_t));
}

void GkFileSystem::WriteFile(const wchar_t* fileName, std::ofstream* fileStream)
{
	std::ifstream file(fileName, std::ios::in | std::ios::binary);

	const int bufferSize = 2048;
	char buffer[bufferSize];

	for (;;)
	{
		file.read(buffer, bufferSize);
		fileStream->write(buffer, file.gcount());

		if (!file)
			break;
	}

	file.close();
}