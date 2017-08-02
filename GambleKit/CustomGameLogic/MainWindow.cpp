#include "MainWindow.h"

#include "Pages\MainPage.h"

MainWindow::MainWindow(void)
{
	if (this->deviceContext == 0)
		GkOSUtils::ShowMessage("Error creating device context", this->handle);
	if (this->renderContext == 0)
		GkOSUtils::ShowMessage("Error creating rendering context", this->handle);

	this->graphicsDevice = new GkGraphicsDevice();
	
	if (this->graphicsDevice->maxTextureSize < 2048)
		GkOSUtils::ShowMessage(GkStrings::ToShort(GkStrings::Format(L"Graphics device does not suport textures with dimensions of size at least 2048 pixels.\r\nMaxium supported size is {0}.\r\nSome textures may not be rendered.", GkStrings::IntToString(this->graphicsDevice->maxTextureSize).c_str()).c_str()).c_str());
	if (!this->graphicsDevice->nonPower2Textures)
		GkOSUtils::ShowMessage("Graphics device does not support textures of arbitraty dimensions. Application will require more video memory than normal.");

	this->soundDevice = new GkSoundDevice();

	GkFileSystem::SetSource(L"..\\GameData\\", FileSystemMode_Physical);

	this->game = new CustomGame(this->graphicsDevice, this->soundDevice);

	this->SetPosition(200, 100);
	// setSize(width,height) //only change on this 
	this->SetSize(800, 600);
	this->SetActive();
}


MainWindow::~MainWindow(void)
{
}

bool MainWindow::Close()
{
	delete this->game;

	delete this->soundDevice;
	delete this->graphicsDevice;

	return true;
}

void MainWindow::MainLoop()
{
	//TODO: timed rendering (main or custom thread, Sleep(frameInterval - renderTime). VSync
	this->game->NewFrame();

	this->MakeCurrent();
	this->game->Process();
	this->game->Render();
	this->Present();

	// If thre are changes to the settings data, they will automatically be written to disk
	//this->game->settings->WriteChanges();
}

void MainWindow::MouseDown(int x, int y, int mouseButtons)
{
	this->game->MouseDown(x, y);
};

void MainWindow::MouseUp(int x, int y, int mouseButtons)
{
	this->game->MouseUp(x, y);
}

void MainWindow::Resize(int width, int height)
{
	this->graphicsDevice->SetViewport(0, 0, width, height);
}

void MainWindow::KeyDown(int keyCode)
{
	if (keyCode == VK_ESCAPE)
		this->DestroyFormWindow();
}