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

#include "GkSoundSource.h"

#include <string>
#include <AL\alut.h>

#pragma pack(1)
struct WAVFileHeader
{
	char chunkID[4];				// "RIFF"
	unsigned int chunkSize;
	char format[4];					// "WAVE"

	char subchunk1Id[4];			// "fmt "
	unsigned int subchunk1Size;
	unsigned short audioFormat;		// 1 - PCM
	unsigned short numChannels;		// 1 - mono, 2 - stereo
	unsigned int sampleRate;		// 22050, 44100, etc.
	unsigned int byteRate;			// == SampleRate * NumChannels * BitsPerSample / 8
	unsigned short blockAlign;		// == NumChannels * BitsPerSample / 8. The number of bytes for one sample including
	unsigned short bitsPerSample;	// 8, 16, etc.
	//unsigned short extraParamsSize;	//if PCM does not exist
	//extraParams;						//variable format

	//char subchunk2Id[4];			// "data"
	//unsigned int subchunk2Size;
};

GkSoundSource::GkSoundSource(void)
{
	alGenSources(1, &this->handle);

	this->buffer = 0;
}


GkSoundSource::~GkSoundSource(void)
{
	this->UnloadBuffer();

	alDeleteSources(1, &this->handle);
}

void GkSoundSource::LoadFromMemory(void *wavData)
{
	ALenum format;
	ALvoid *data;
	ALsizei size, frequency;

	WAVFileHeader *header = (WAVFileHeader*)wavData;
	int dataOffset = 20 + header->subchunk1Size;
	while (memcmp((char*)&((char*)wavData)[dataOffset], &"data", 4) != 0)
	{
		dataOffset += *((int*)&(((char*)wavData)[dataOffset + 4])) + 8;
	}
	size = *((int*)&(((char*)wavData)[dataOffset + 4]));
	data = (ALvoid*)&(((char*)wavData)[dataOffset + 8]);
	format = header->numChannels == 1 ? header->bitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16 : header->bitsPerSample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
	frequency = header->sampleRate;

	this->UnloadBuffer();

	alGenBuffers(1, &this->buffer);
	alBufferData(this->buffer, format, data, size, frequency);

	alSourcei(this->handle, AL_BUFFER, this->buffer);
}

void GkSoundSource::UnloadBuffer()
{
	if (this->buffer == 0)
		return;

	alDeleteBuffers(1, &this->buffer);
	this->buffer = 0;
}
