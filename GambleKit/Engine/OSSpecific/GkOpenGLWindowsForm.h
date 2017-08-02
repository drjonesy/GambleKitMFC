#pragma once

#include "..\GkDefinitions.h"

#include "GkWindowsForm.h"

class GKDLLCLASS GkOpenGLWindowsForm : 
	public GkWindowsForm
{
public:
	HDC deviceContext;
	HGLRC renderContext;

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	unsigned int pixelFormat;

	GkOpenGLWindowsForm(HGLRC renderContext = 0);
	virtual ~GkOpenGLWindowsForm(void);

	bool GkOpenGLWindowsForm::CreateContext(HGLRC renderContext = 0);
	bool GkOpenGLWindowsForm::Present();
	bool GkOpenGLWindowsForm::MakeCurrent();
};

