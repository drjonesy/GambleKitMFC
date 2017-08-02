//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

// #include <vld\vld.h>

#include "MainWindow.h"

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GkMaths::Randomize();

	//Dictionary::Initialize();
	GkImage::Initialize();
	GkGraphicsDevice::Initialize();
	GkSoundDevice::Initialize();

	MainWindow mainWindow;
	
	GkWindowsForm::PerformFormLoop();

	GkSoundDevice::Close();
	//Dictionary::Close();

	//_CrtDumpMemoryLeaks();

	return 0;
}