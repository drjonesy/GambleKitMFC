#pragma once

#include "..\Engine\GambleKit.h"

#include "CustomGame.h"

class MainWindow :
	public GkOpenGLWindowsForm
{
public:
	GkGraphicsDevice *graphicsDevice;
	GkSoundDevice *soundDevice;

	CustomGame *game;

	MainWindow::MainWindow(void);
	MainWindow::~MainWindow(void);

	bool MainWindow::Close();
	void MainWindow::MainLoop();
	void MainWindow::MouseDown(int x, int y, int mouseButtons);
	void MainWindow::MouseUp(int x, int y, int mouseButtons);
	void MainWindow::Resize(int width, int height);
	void MainWindow::KeyDown(int keyCode);
};

