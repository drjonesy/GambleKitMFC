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

#include "GkOSUtils.h"

#include <stdlib.h>
#include <time.h>

GkOSUtils::GkOSUtils(void)
{
}


GkOSUtils::~GkOSUtils(void)
{
}


void* GkOSUtils::GetMem(int size)
{
	return malloc(size);
}

void GkOSUtils::FreeMem(void* data)
{
	free(data);
}

void GkOSUtils::ZeroMem(void* data, int size)
{
	memset(data, 0, size);
}

void GkOSUtils::CopyMem(void* destination, void* source, int size)
{
	memcpy(destination, source, size);
}

int GkOSUtils::CompareMem(void* block1, void* block2, int size)
{
	return memcmp(block1, block2, size);
}

long long GkOSUtils::GetPerformanceCounter()
{
	LARGE_INTEGER result;

	if (QueryPerformanceCounter(&result))
		return result.QuadPart;
	else
		return -1;
}

long long GkOSUtils::GetPerformanceCounterFrequency()
{
	LARGE_INTEGER result;

	if (QueryPerformanceFrequency(&result))
		return result.QuadPart;
	else
		return -1;
}

void GkOSUtils::ShowMessage(const char *text, HWND windowHandle)
{
	MessageBox(windowHandle, text, "Message", MB_OK);
}

int GkOSUtils::GetScreenWidth()
{
	return GetSystemMetrics(SM_CXSCREEN);
}

int GkOSUtils::GetScreenHeight()
{
	return GetSystemMetrics(SM_CYSCREEN);
}


BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    int *Count = (int*)dwData;
    (*Count)++;
    return TRUE;
}

int GkOSUtils::GetNumberOfScreens()
{
    int Count = 0;
    if (EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&Count))
        return Count;
    return -1;//signals an error
}

DateTime GkOSUtils::GetDateTime()
{
	time_t currentTime;
	time(&currentTime);

	tm localTime;
	localtime_s(&localTime, &currentTime);

	DateTime result;
	result.day = localTime.tm_mday;
	result.month = 1 + localTime.tm_mon;
	result.year = 1900 + localTime.tm_year;
	result.hour = localTime.tm_hour;
	result.minute = localTime.tm_min;
	result.second = localTime.tm_sec;

	return result;
}

bool GkOSUtils::SetDateTime(DateTime &dateTime)
{
	SYSTEMTIME sysTime;
	sysTime.wDay = dateTime.day;
	sysTime.wMonth = dateTime.month;
	sysTime.wYear = dateTime.year;
	sysTime.wHour = dateTime.hour;
	sysTime.wMinute = dateTime.minute;
	sysTime.wSecond = dateTime.second;
	sysTime.wMilliseconds = 0;

	return SetLocalTime(&sysTime) != 0;
}

int GkOSUtils::GetSecondsElapsed(DateTime &since)
{
	//TODO
	
	DateTime now = GkOSUtils::GetDateTime();
	return ((now.hour - since.hour) * 60 + (now.minute - since.minute)) * 60 + (now.second - since.second);
}

DateTime GkOSUtils::GetFileModificationDateTime(const wchar_t *fileName)
{
	DateTime result;

	HANDLE handle = CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (handle != INVALID_HANDLE_VALUE) 
	{
		FILETIME created, accessed, modified;

		GetFileTime(handle, &created, &accessed, &modified);
	
		FILETIME localFileTime;
		FileTimeToLocalFileTime(&modified, &localFileTime);

		SYSTEMTIME localTime;
		FileTimeToSystemTime(&localFileTime, &localTime);

		result.day = localTime.wDay;
		result.month = localTime.wMonth;
		result.year = localTime.wYear;
		result.hour = localTime.wHour;
		result.minute = localTime.wMinute;
		result.second = localTime.wSecond;
	}
	CloseHandle(handle);

	return result;
}

std::string GkOSUtils::GetApplicationName()
{
	TCHAR buffer[MAX_PATH]={0};
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return std::string(buffer);
}

std::string GkOSUtils::GetApplicationPath()
{
	std::string applicationName = GkOSUtils::GetApplicationName();
	int nameLength = applicationName.length();
	char path[MAX_PATH];
	int pathLast = 0;
	for (int i = 0; i < nameLength; i++)
	{
		path[i] = applicationName[i];
		if (path[i] == '\\')
			pathLast = i;
	}
	path[pathLast + 1] = 0;

	return std::string(path);
}

void GkOSUtils::Execute(const char *command)
{
	ShellExecuteA(GetDesktopWindow(), "open", command, NULL, NULL, SW_SHOWNORMAL);
}