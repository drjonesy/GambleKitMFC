#pragma once

#include "..\GkDefinitions.h"

#include <windows.h>

class GKDLLCLASS GkWindowsMutex
{
public:
    HANDLE handle;

	GkWindowsMutex(void);
	~GkWindowsMutex(void);

	bool Lock();
	void Unlock();
};

