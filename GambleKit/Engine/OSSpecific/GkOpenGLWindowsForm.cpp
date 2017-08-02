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

#include "GkOpenGLWindowsForm.h"


GkOpenGLWindowsForm::GkOpenGLWindowsForm(HGLRC renderContext)
{
	PIXELFORMATDESCRIPTOR d =
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	this->pixelFormatDescriptor = d;

	this->CreateContext(renderContext);
}


GkOpenGLWindowsForm::~GkOpenGLWindowsForm(void)
{
}

bool GkOpenGLWindowsForm::CreateContext(HGLRC renderContext)
{
	this->deviceContext = GetDC(this->handle);
	if (this->deviceContext == 0)
		return false;

	this->pixelFormat = ChoosePixelFormat(this->deviceContext, &this->pixelFormatDescriptor);
	if (this->pixelFormat == 0)
		return false;

	if(!SetPixelFormat(this->deviceContext, this->pixelFormat, &this->pixelFormatDescriptor))
		return false;

	if (renderContext == 0)
	{
		this->renderContext = wglCreateContext(this->deviceContext);
		if (this->renderContext == 0)
			return false;
	}
	else 
	{
		this->renderContext = renderContext;
	}

	if (!this->MakeCurrent())
		return false;

	return true;
}

bool GkOpenGLWindowsForm::Present()
{
	return SwapBuffers(this->deviceContext) != 0;
}

bool GkOpenGLWindowsForm::MakeCurrent()
{
	return wglMakeCurrent(this->deviceContext, this->renderContext) != 0;
}