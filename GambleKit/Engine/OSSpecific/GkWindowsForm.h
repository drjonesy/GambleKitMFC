#pragma once

#include "..\GkDefinitions.h"

#include "..\Utils\GkList.h"
//#include <vector>

#include <windows.h>

class GKDLLCLASS GkWindowsForm
{
public:
	static const int WindowMode_Windowed = 0,
					 WindowMode_FullScreen = 1,
					 
					 MouseButton_None = 0,
					 MouseButton_Left = 1,
					 MouseButton_Right = 2,
					 MouseButton_Middle = 4;

	static const char* FormClassName;

	static GkList<GkWindowsForm*> instances;
	//static std::vector<GkWindowsForm*> instances;


	HINSTANCE applicationInstance;
	WNDCLASSEX windowClass;
	HWND handle;

	bool keys[256];
	int mouseX, mouseY,	mouseButtons;
	bool suppressScreenSaver, suppressMonitorPowerSave;
	
	bool active;
	int windowMode;
	int left, top, width, height;
	char *caption;


	GkWindowsForm(void);
	virtual ~GkWindowsForm(void);

	bool GkWindowsForm::RegisterWindowClass();
	bool GkWindowsForm::CreateFormWindow();
	void GkWindowsForm::ShowFormWindow();
	void GkWindowsForm::DestroyFormWindow();
	void GkWindowsForm::SetPosition(int left, int top);
	void GkWindowsForm::SetSize(int width, int height);
	void GkWindowsForm::SetStyle();
	void GkWindowsForm::SetActive();

	virtual bool GkWindowsForm::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, int *result) { return false; };
	virtual void GkWindowsForm::MainLoop() { };
	virtual void GkWindowsForm::Created() { };
	virtual bool GkWindowsForm::Close() { return true; };
	virtual bool GkWindowsForm::Paint() { return true; };
	virtual void GkWindowsForm::KeyDown(int keyCode) { };
	virtual void GkWindowsForm::KeyUp(int keyCode) { };
	virtual void GkWindowsForm::Resize(int width, int height) { };
	virtual void GkWindowsForm::MouseMove(int x, int y) { };
	virtual void GkWindowsForm::MouseDown(int x, int y, int mouseButton) { };
	virtual void GkWindowsForm::MouseUp(int x, int y, int mouseButton) { };
	virtual void GkWindowsForm::MouseWheel(int x, int y, int deltaFactor, int delta) { };

	static LRESULT CALLBACK GkWindowsForm::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static GkWindowsForm* GkWindowsForm::GetInstance(HWND instanceHandle);
	static bool GkWindowsForm::ProcessMessages();
	static int GkWindowsForm::PerformFormLoop();
};

