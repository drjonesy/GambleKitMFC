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

#include "GkGamePage.h"

GkGamePage::GkGamePage()
{
}


GkGamePage::~GkGamePage(void)
{
	int numEntities = this->entities.size();

	for (int i = 0; i < numEntities; i++)
	{
		delete this->entities[i];
	}
}

void GkGamePage::OnShow(GkGamePage *previous)
{
}

void GkGamePage::OnHide(GkGamePage *next)
{
}

GkGameEntity* GkGamePage::AddEntity(GkGameEntity *entity)
{
	this->entities.push_back(entity);

	return entity;
}

GkGameEntity* GkGamePage::InsertEntity(int position, GkGameEntity *entity)
{
	this->entities.push_back(0);

	for (int i = this->entities.size() - 1; i > position; i--)
		this->entities[i] = this->entities[i - 1];

	this->entities[position] = entity;

	return entity;
}

bool GkGamePage::RemoveEntity(GkGameEntity *entity)
{
	int numEntities = this->entities.size();

	for (int i = 0; i < numEntities; i++)
	{
		if (this->entities[i] == entity)
		{
			this->entities[i] = this->entities[numEntities - 1];
			this->entities.pop_back();

			return true;
		}	
	}

	return false;
}

void GkGamePage::Process(GkGame *game)
{
	int numEntities = this->entities.size();

	for (int i = 0; i < numEntities; i++)
	{
		this->entities[i]->Process(game);
	}
}

void GkGamePage::Render(GkGame *game)
{
	int numEntities = this->entities.size();

	for (int i = 0; i < numEntities; i++)
	{
		this->entities[i]->Draw(game);
	}
}
