#pragma once

#include "..\GkDefinitions.h"

#include <windows.h>
#include <string>

struct DateTime
{
	int day, month, year,
		hour, minute, second;
};

class GKDLLCLASS GkOSUtils
{
public:
	GkOSUtils(void);
	~GkOSUtils(void);

	static void* GkOSUtils::GetMem(int size);
	static void GkOSUtils::FreeMem(void* data);
	static void GkOSUtils::ZeroMem(void* data, int size);
	static void GkOSUtils::CopyMem(void* destination, void* source, int size);
	static int GkOSUtils::CompareMem(void* block1, void* block2, int size);

	static long long GkOSUtils::GetPerformanceCounter();
	static long long GkOSUtils::GetPerformanceCounterFrequency();

	static void GkOSUtils::ShowMessage(const char *text, HWND windowHandle = 0);

	static int GetScreenWidth();
	static int GetScreenHeight();

	static int GetNumberOfScreens();

	static DateTime GetDateTime();
	static bool SetDateTime(DateTime &dateTime);
	static int GetSecondsElapsed(DateTime &since);

	static DateTime GetFileModificationDateTime(const wchar_t *fileName);

	static std::string GetApplicationName();
	static std::string GetApplicationPath();

	static void Execute(const char *command);
};

