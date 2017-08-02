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

#include "GkWindowsThread.h"


DWORD WINAPI GkWindowsThread::ExecuteThread(LPVOID lpParam)
{
	GkWindowsThread *instance = (GkWindowsThread*)lpParam;

	int result = instance->Execute();
	instance->exited = true;

	return result;
}

void GkWindowsThread::SleepX(int milliseconds)
{
	Sleep(milliseconds);
}

GkWindowsThread::GkWindowsThread(void)
{
	this->handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) GkWindowsThread::ExecuteThread, this, 0, &this->id);
    this->terminated = this->handle == NULL;
    this->exited = terminated;
}


GkWindowsThread::~GkWindowsThread(void)
{
	if(this->handle == NULL)
		return;

	this->terminated = true;

	while (!this->exited)
		Sleep(0);

	CloseHandle(this->handle);
}