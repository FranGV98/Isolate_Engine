#include <fstream>
#include <filesystem>

#include "PhysFS/include/physfs.h"

#include "Application.h"
#include "Globals.h"

#include "FileSystem.h"

#pragma comment (lib, "PhysFS/libx86/physfs.lib")

FileSystem::FileSystem(bool start_enabled) : Module(start_enabled)
{
	// Initializing PhysFS before Init() is required for other modules to be able to use it!
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(nullptr);
	SDL_free(base_path);

	// Setting the working directory as the writing directory.
	RESULT result = (RESULT)PHYSFS_setWriteDir(".");							// Defines where Physfs writes the files
	if (result == RESULT::FAILURE)
	{
		LOG("Can not creat write directory: %s\n", PHYSFS_getLastError());
	}
}

FileSystem::~FileSystem()
{
	PHYSFS_deinit();
}

bool FileSystem::Init()
{
	LOG("Loading File System")

		return true;
}

bool FileSystem::CleanUp()
{
	bool ret = true;

	return ret;
}

bool FileSystem::CheckFile(const char* file) const
{
	RESULT result = (RESULT)PHYSFS_exists(file);								// Method that checks whether or not a file is in the search path. Returns 0 upon failure.

	if (result == RESULT::SUCCESS)
	{
		return true;
	}
	else
	{
		LOG("File %s does not exist within PhysFS search path.", file);

		return false;
	}
}

uint FileSystem::Load(const char* path, const char* file, char** buffer) const
{
	//Joins the path and the file into one single string
	std::string full_path(path);

	full_path += file;

	uint ret = 0;
	const char* file_path = full_path.c_str();

	PHYSFS_file* fs_file = PHYSFS_openRead(file_path);

	if (fs_file != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fs_file);

		if (size > 0)
		{
			*buffer = new char[size + 1];

			uint amount_read = (uint)PHYSFS_read(fs_file, *buffer, 1, size);

			if (amount_read != size)
			{
				LOG("File Syste, error while reading from file %s: %s\n", file_path, PHYSFS_getLastError());
				RELEASE_ARRAY(buffer);
			}
			else
			{
				ret = amount_read;

				(*buffer)[size] = '\0';
			}
		}

		if (PHYSFS_close(fs_file) == (int)RESULT::FAILURE)
		{
			LOG("File System error while closing file %s: %s", file_path, PHYSFS_getLastError());
		}
	}
	else
	{
		LOG("File System error while opening file %s: %s", file_path, PHYSFS_getLastError());
	}

	return ret;



}

uint FileSystem::Save(const char* file, const void* buffer, uint size, bool append) const
{
	uint ret = 0;

	bool overwrite = CheckFile(file);															

	PHYSFS_file* fs_file;																	
	
	//Opens the file for either Appending or Writing
	if (append)
	{
		fs_file = PHYSFS_openAppend(file);
	}
	else
	{
		fs_file = PHYSFS_openWrite(file);
	}

	if (fs_file != nullptr)
	{
		uint write_call = (uint)PHYSFS_write(fs_file, buffer, 1, size);	
		
		if (write_call != size)																
		{
			LOG("File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		}
		else
		{
			if (append)
			{
				LOG("Appended/Added %u data to [%s%s]", size, PHYSFS_getWriteDir(), file);
			}
			else if (overwrite)
			{
				LOG("File [%s%s] overwritten with %u bytes", PHYSFS_getWriteDir(), file, size);
			}
			else
			{
				LOG("New file created [%s%s] of %u bytes", PHYSFS_getWriteDir(), file, size);
			}

			ret = write_call;
		}

		RESULT result = (RESULT)PHYSFS_close(fs_file);						
		if (result == RESULT::FAILURE)
		{
			LOG("File System error while closing file %s: %s", file, PHYSFS_getLastError());
		}
	}
	else
	{
		LOG("File System error while opening file %s: %s", file, PHYSFS_getLastError());
	}

	return ret;
}




