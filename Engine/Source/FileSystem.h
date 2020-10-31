#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <vector>
#include "Module.h"


class FileSystem : public Module
{
public:
	FileSystem(bool is_active = true);
	~FileSystem();

	bool Init() override;
	bool CleanUp() override;

public:
	//File management

	bool CheckFile(const char* file) const;

	//Load and Save files

	uint Load(const char* path, const char* file, char** buffer) const;		
	uint Save(const char* file, const void* buffer, uint size, bool append = false) const;	


};

#endif // !__File_SYSTEM_H__
