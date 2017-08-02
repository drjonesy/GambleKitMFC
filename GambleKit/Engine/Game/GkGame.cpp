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

#include "GkGame.h"

#include "..\OSSpecific\GkFileSystem.h"
#include "..\Utils\GkStrings.h"


ResourcePoolEntry::ResourcePoolEntry(GkBase *resource, const wchar_t *id)
{
	this->resource = resource;
	
	if (id == NULL)
	{
		this->id = NULL;
	}
	else
	{
		int l = wcslen(id);
		this->id = new wchar_t[l + 1];
		wcscpy_s(this->id, l + 1, id);
	}
}

GkGame::GkGame(GkGraphicsDevice *graphicsDevice, GkSoundDevice *soundDevice)
{
	this->graphicsDevice = graphicsDevice;
	this->soundDevice = soundDevice;

	this->currentGamePage = 0;
	
	this->timer = new GkGameTimer();
}


GkGame::~GkGame(void)
{
	int numPages = this->gamePages.size();
	for (int i = 0; i < numPages; i++)
	{
		delete this->gamePages[i];
	}

	int numResources = this->resourcePool.size();
	for (int i = 0; i < numResources; i++)
	{
		delete this->resourcePool[i].resource;
		
		if (this->resourcePool[i].id != NULL)
		{
			delete this->resourcePool[i].id;
			this->resourcePool[i].id = NULL;
		}
	}

	delete this->timer;
}

void GkGame::SetPage(GkGamePage *page)
{
	if (this->currentGamePage != 0)
		this->currentGamePage->OnHide(page);

	if (page != 0)
		page->OnShow(this->currentGamePage);

	this->currentGamePage = page;
}

GkAnimatedTexture* GkGame::AddAnimatedTexture(GkAnimatedTexture *animatedTexture, const wchar_t* id)
{
	this->resourcePool.push_back(ResourcePoolEntry(animatedTexture, id));
	return animatedTexture;
}

void* GkGame::GetResource(const wchar_t* id)
{
	int numResources = this->resourcePool.size();
	for (int i = 0; i < numResources; i++)
	{
		if (wcscmp(this->resourcePool[i].id, id) == 0)
			return this->resourcePool[i].resource;
	}

	return NULL;
}

bool GkGame::ReleaseResource(const wchar_t* id)
{
	int numResources = this->resourcePool.size();
	for (int i = 0; i < numResources; i++)
	{
		if (wcscmp(this->resourcePool[i].id, id) == 0)
		{
			delete this->resourcePool[i].resource;
		
			if (this->resourcePool[i].id != NULL)
			{
				delete this->resourcePool[i].id;
				this->resourcePool[i].id = NULL;
			}

			this->resourcePool[i] = this->resourcePool[numResources - 1];
			this->resourcePool.pop_back();

			return true;
		}
	}

	return false;
}

GkAnimatedFont* GkGame::AcquireAnimatedFontFromMultipleFiles(const wchar_t* virtualFileName, const wchar_t *charMap, int numFrames)
{
	GkAnimatedFont* result = (GkAnimatedFont*)this->GetResource(virtualFileName);
	if (result != NULL)
		return result;

	
	result = new GkAnimatedFont(charMap, numFrames);

	void *data;
	unsigned int dataSize;

	for (int i = 0; i < result->numLetters; i++)
	{
		if (GkFileSystem::LoadFile(GkStrings::Format(virtualFileName, GkStrings::IntToString(i, 10, 5).c_str()).c_str(), &data, &dataSize))
		{
			result->LoadAnimatedLetter(i, data, dataSize);
			
			delete[] data;
		}
	}


	this->resourcePool.push_back(ResourcePoolEntry(result, virtualFileName));

	return result;
}

GkAnimatedTexture* GkGame::AcquireAnimatedTexture(const wchar_t* virtualFileName, int columnFrames, int rowFrames, int numFrames, float framesPerSecond, int animatedTexture_LoadOrder)
{
	GkAnimatedTexture* result = (GkAnimatedTexture*)this->GetResource(virtualFileName);
	if (result != NULL)
		return result;

	void *data;
	unsigned int dataSize;

	if (!GkFileSystem::LoadFile(virtualFileName, &data, &dataSize))
		return NULL;
	result = new GkAnimatedTexture(data, dataSize, columnFrames, rowFrames, numFrames, framesPerSecond, animatedTexture_LoadOrder);
	delete data;

	this->resourcePool.push_back(ResourcePoolEntry(result, virtualFileName));

	return result;
}

GkAnimatedTexture* GkGame::AcquireAnimatedTextureFromMultipleFiles(const wchar_t* virtualFileName, float framesPerSecond)
{
	GkAnimatedTexture *result = (GkAnimatedTexture*)this->GetResource(virtualFileName);
	if (result != NULL)
		return result;


	std::vector<GkTexture*> frames;

	void *data;
	unsigned int dataSize;

	int index = 0;
	while (GkFileSystem::LoadFile(GkStrings::Format(virtualFileName, GkStrings::IntToString(index, 10, 5).c_str()).c_str(), &data, &dataSize))
	{
		GkImage *frameImage = new GkImage();
		frameImage->LoadFromMemory(data, dataSize);

		GkTexture *frame = new GkTexture();
		frame->LoadFromImage(frameImage);
		delete frameImage;

		frames.push_back(frame);

		index++;

		delete[] data;
	}


	int numFrames = frames.size();
	if (numFrames == 0)
		return NULL;

	result = new GkAnimatedTexture();
	result->framesPerSecond = framesPerSecond;
	result->numFrames = numFrames;
	result->frames = new GkTexture*[numFrames];
	for (int i = 0; i < numFrames; i++)
	{
		result->frames[i] = frames[i];
	}

	this->resourcePool.push_back(ResourcePoolEntry(result, virtualFileName));

	return result;
}

GkSoundSource* GkGame::AcquireSoundSource(const wchar_t* virtualFileName)
{
	GkSoundSource* result = (GkSoundSource*)this->GetResource(virtualFileName);
	if (result != NULL)
		return result;

	void *data;
	unsigned int dataSize;

	GkFileSystem::LoadFile(virtualFileName, &data, &dataSize);
	result = new GkSoundSource();
	result->LoadFromMemory(data);
	this->resourcePool.push_back(ResourcePoolEntry(result, virtualFileName));
	delete data;

	return result;
}

GkGamePage* GkGame::AddGamePage(GkGamePage *gamePage)
{
	this->gamePages.push_back(gamePage);

	if (this->currentGamePage == NULL)
		this->SetPage(gamePage);

	return gamePage;
}

void GkGame::RemoveGamePage(GkGamePage *gamePage)
{
	int index = -1;
	for (int i = 0; i < (int)this->gamePages.size(); i++)
		if (this->gamePages[i] == gamePage)
		{
			index = i;
			break;
		}

	if (index == -1)
		return;

	for (int i = index; i < (int)this->gamePages.size() - 1; i++)
		this->gamePages[i] = this->gamePages[i + 1];

	this->gamePages.pop_back();
}

void GkGame::MouseDown(int x, int y)
{
	int numEntities = this->currentGamePage->entities.size();
	for (int i = numEntities - 1; i >= 0; i--)
	{
		GkGameEntity *entity = this->currentGamePage->entities[i];
		if (entity->IsMouseOver(x, y))
		{
			entity->MouseDown(x, y);
			break;
		}
	}
}

void GkGame::MouseUp(int x, int y)
{
	int numEntities = this->currentGamePage->entities.size();
	for (int i = numEntities - 1; i >= 0; i--)
	{
		GkGameEntity *entity = this->currentGamePage->entities[i];
		entity->MouseUp(x, y);
	}
}

void GkGame::NewFrame()
{
	this->timer->Tick();
}

void GkGame::Process()
{
	if (this->currentGamePage != NULL)
		this->currentGamePage->Process(this);
}

void GkGame::Render()
{
	if (this->currentGamePage != NULL)
		this->currentGamePage->Render(this);
}