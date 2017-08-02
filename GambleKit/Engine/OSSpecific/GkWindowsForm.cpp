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

#include "GkWindowsForm.h"

#include <windowsx.h>

const char* GkWindowsForm::FormClassName = "MyWindowsForm";

//std::vector<GkWindowsForm*> GkWindowsForm::instances;
GkList<GkWindowsForm*> GkWindowsForm::instances;

LRESULT CALLBACK GkWindowsForm::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	GkWindowsForm* instance = GkWindowsForm::GetInstance(hWnd);

	if (instance != NULL)
	{
		switch (uMsg)									
		{
			case WM_SYSCOMMAND:							
			{
				switch (wParam)							
				{
					case SC_SCREENSAVE:
						{
							if (instance->suppressScreenSaver)
								return 0;
						}
					case SC_MONITORPOWER:
						{
							if (instance->suppressMonitorPowerSave)
								return 0;
						}
				}
				break;									
			}

			case WM_ACTIVATE:							
			{
				// Minimization state
				if (!HIWORD(wParam))					
				{
					instance->active=TRUE;				
				}
				else
				{
					instance->active=FALSE;				
				}

				return 0;								
			}

			case WM_DESTROY:
			{
				if (instance->Close())
				{
					GkWindowsForm::instances.Remove(instance);

					if (GkWindowsForm::instances.size == 0)
						PostQuitMessage(0);
				}
				return 0;
			}

			case WM_SIZE:								
			{
				int width = LOWORD(lParam);
				int height = HIWORD(lParam);

				instance->Resize(width, height);

				return 0;								
			}

			case WM_PAINT:
				{
					if (instance->Paint())
					{
						ValidateRect(instance->handle, NULL);
					}

					return 0;
				}

			case WM_KEYDOWN:						
			{
				int key = wParam;

				instance->KeyDown(key);

				if ((key >=0 ) && (key < 256))
					instance->keys[key] = TRUE;			
				return 0;							
			}

			case WM_KEYUP:								
			{
				int key = wParam;

				instance->KeyUp(key);

				if ((key >=0 ) && (key < 256))
					instance->keys[key] = FALSE;				
				return 0;								
			}

			case WM_LBUTTONDOWN:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseDown(x, y, GkWindowsForm::MouseButton_Left);
					instance->mouseButtons |= GkWindowsForm::MouseButton_Left;

					return 0;
				}

			case WM_LBUTTONUP:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseUp(x, y, GkWindowsForm::MouseButton_Left);
					instance->mouseButtons &= !GkWindowsForm::MouseButton_Left;

					return 0;
				}

			case WM_RBUTTONDOWN:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseDown(x, y, GkWindowsForm::MouseButton_Right);
					instance->mouseButtons |= GkWindowsForm::MouseButton_Right;

					return 0;
				}

			case WM_RBUTTONUP:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseUp(x, y, GkWindowsForm::MouseButton_Right);
					instance->mouseButtons &= !GkWindowsForm::MouseButton_Right;

					return 0;
				}

			case WM_MBUTTONDOWN:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseDown(x, y, GkWindowsForm::MouseButton_Middle);
					instance->mouseButtons |= GkWindowsForm::MouseButton_Middle;

					return 0;
				}

			case WM_MBUTTONUP:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseUp(x, y, GkWindowsForm::MouseButton_Middle);
					instance->mouseButtons &= !GkWindowsForm::MouseButton_Middle;

					return 0;
				}

			case WM_MOUSEMOVE:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseMove(x, y);

					instance->mouseX = x;
					instance->mouseY = y;

					return 0;
				}

			case WM_MOUSEHWHEEL:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);
					int deltaFactor = GET_WHEEL_DELTA_WPARAM(wParam);

					instance->MouseWheel(x, y, deltaFactor, WHEEL_DELTA);

					return 0;
				}
		}

		int result;
		if (instance->HandleMessage(uMsg, wParam, lParam, &result))
			return result;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

GkWindowsForm* GkWindowsForm::GetInstance(HWND instanceHandle)
{
	for (int i = 0; i < GkWindowsForm::instances.size; i++)
	{
		if (GkWindowsForm::instances[i]->handle == instanceHandle)
		{
			return GkWindowsForm::instances[i];
		}
	}

	return NULL;
}

bool GkWindowsForm::ProcessMessages()
{
    MSG msg;

	bool result = false;

	while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

		result = true;
    }

	return result;
}

int GkWindowsForm::PerformFormLoop()
{
    MSG msg;

	bool done = false;
	while(!done)										// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)					// Have We Received A Quit Message?
			{
				done = true;							// If So done=TRUE
			}
			else										// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);					// Translate The Message
				DispatchMessage(&msg);					// Dispatch The Message
			}
		}

		if (!done)
		{
			for (int i = 0; i < GkWindowsForm::instances.size; i++)
			{
				GkWindowsForm::instances[i]->MainLoop();
			}
		}
	}

	return (int)msg.wParam;
		
	//while (GetMessage(&msg, NULL, 0, 0))
 //   {
 //       TranslateMessage(&msg);
 //       DispatchMessage(&msg);
 //   }

 //   return (int)msg.wParam;
}

GkWindowsForm::GkWindowsForm(void)
{
	this->applicationInstance = GetModuleHandle(NULL);
	ZeroMemory(keys, 256);

	this->left = 0;
	this->top = 0;
	this->width = 640;
	this->height = 480;
	this->caption = "Window Title";
	//this->windowMode = GkWindowsForm::WindowMode_Windowed;
	this->windowMode = GkWindowsForm::WindowMode_FullScreen;

	GkWindowsForm::instances.Add(this);
	//GkWindowsForm::instances.push_back(this);

	this->RegisterWindowClass();
	this->CreateFormWindow();
	this->ShowFormWindow();
}


GkWindowsForm::~GkWindowsForm(void)
{
	this->DestroyFormWindow();
	//GkWindowsForm::instances.Remove(this);
	//if (GkWindowsForm::instances.size == 0)
	//	PostMessage(this->handle, WM_CLOSE, 0, 0);
}

bool GkWindowsForm::RegisterWindowClass()
{
	//LRESULT (CALLBACK GkWindowsForm::*memfun)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//memfun = &GkWindowsForm::WndProc;
    //this->ex.lpfnWndProc = *((WNDPROC*) (&memfun));

	this->windowClass.cbSize = sizeof(WNDCLASSEX);
    this->windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	this->windowClass.lpfnWndProc = (WNDPROC)GkWindowsForm::WndProc;
    this->windowClass.cbClsExtra = 0;
    this->windowClass.cbWndExtra = 0;
    this->windowClass.hInstance = this->applicationInstance;
    this->windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    this->windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    this->windowClass.hbrBackground = NULL;
    this->windowClass.lpszMenuName = NULL;
    this->windowClass.hIconSm = NULL;
	this->windowClass.lpszClassName = GkWindowsForm::FormClassName;

	return RegisterClassEx(&this->windowClass) != 0;
}

bool GkWindowsForm::CreateFormWindow()
{
    long wndStyle = this->windowMode == GkWindowsForm::WindowMode_FullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
	wndStyle |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	// Translate from client rect to window rect
	RECT wRect = { this->left, this->top, this->left + this->width, this->top + this->height };
	AdjustWindowRect(&wRect, wndStyle, false);

	this->handle = CreateWindow(GkWindowsForm::FormClassName, this->caption,
							    wndStyle,
								this->left, this->top, wRect.right - wRect.left, wRect.bottom - wRect.top,
							    NULL, NULL,
								this->applicationInstance,
							    NULL);

	return this->handle != 0;
}

void GkWindowsForm::ShowFormWindow()
{
	ShowWindow(this->handle,SW_SHOW);
    //UpdateWindow(this->handle);
	SetForegroundWindow(this->handle);						// Slightly Higher Priority
	SetFocus(this->handle);									// Sets Keyboard Focus To The Window
}

void GkWindowsForm::DestroyFormWindow()
{
	PostMessage(this->handle, WM_DESTROY, 0, 0);
}

void GkWindowsForm::SetPosition(int left, int top)
{
	this->left = left;
	this->top = top;

	SetWindowPos(this->handle, HWND_TOP, this->left, this->top, this->width, this->height, SWP_NOSIZE);
}

void GkWindowsForm::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;

	SetWindowPos(this->handle, HWND_TOP, this->left, this->top, this->width, this->height, SWP_NOMOVE);
}

void GkWindowsForm::SetStyle()
{
	SetWindowLong(this->handle, GWL_STYLE, WS_POPUP | WS_MINIMIZEBOX);
	SetWindowLong(this->handle, GWL_EXSTYLE, 0);
	//SetWindowLong(this->handle, GWL_STYLE, WS_POPUP);
	//SetWindowLong(this->handle, GWL_EXSTYLE, WS_EX_APPWINDOW);
}

void GkWindowsForm::SetActive()
{
	SetActiveWindow(this->handle);
}
