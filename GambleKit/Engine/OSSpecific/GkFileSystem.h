#pragma once

#include "..\GkDefinitions.h"

#include <fstream>
#include <string>

enum FileSystemMode
{
	FileSystemMode_Physical,
	FileSystemMode_Virtual
};

class GKDLLCLASS GkFileSystem
{
private:
	static class GKDLLCLASS Init
	{
	public:
		Init(void);
		~Init(void);
	} init;

	static std::wstring source;
	//static wchar_t *source;
	static FileSystemMode mode;

	static wchar_t* ReadString(std::ifstream* fileStream);

	static void WriteVirtualFileSystem(const wchar_t* directory, std::ofstream* fileStream);

	static void WriteString(const wchar_t* s, std::ofstream* fileStream);
	static void WriteFile(const wchar_t* fileName, std::ofstream* fileStream);

	static bool LoadPhysicalFile(const wchar_t *fileName, void **fileData, unsigned int *fileSize);
	static bool LoadVirtualFile(const wchar_t *fileName, void **fileData, unsigned int *fileSize);

public:
	GkFileSystem(void);
	~GkFileSystem(void);

	// Sets the source and mode of the virtual file system. 
	// If mode is FileSystemMode_Virtual, the source should be the name of the file, containing the virtual file system. Such file can be produced by the CreateVirtualFileSystem method.
	// If mode is FileSystemMode_Physical, the source can be a root directory for the game's resources. Usually, this root directory is used to create a virtual file system from.
	static void SetSource(const wchar_t *source, FileSystemMode mode);
	
	// Allocates memory in fileData and loads the requested file in it, returning the size of the file in fileSize
	static bool LoadFile(const wchar_t *fileName, void **fileData, unsigned int *fileSize);

	// Writes the contents of a directory and all its subdirectories into a virtual file with the given name. The file can then be used as a virtual file system file.
	static void CreateVirtualFileSystem(const wchar_t* directory, const wchar_t* fileName);
};

