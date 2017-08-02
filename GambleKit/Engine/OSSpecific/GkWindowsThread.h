#pragma once

#include "..\GkDefinitions.h"

#include <windows.h>

class GKDLLCLASS GkWindowsThread
{
public:
    HANDLE handle;
    DWORD id;

	bool terminated, exited;

	GkWindowsThread(void);
	virtual ~GkWindowsThread(void);

	virtual int Execute() = 0;

	void Lock();
	void Unlock();

	static void SleepX(int milliseconds);
	static DWORD WINAPI ExecuteThread(LPVOID lpParam);
};

