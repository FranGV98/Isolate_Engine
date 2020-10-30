
#include "Globals.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"
#include "glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "DevIL/include/ilut.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/il.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "glew/libx86/glew32.lib")

struct MeshData
{
	enum Buffers
	{
		index,
		normal,
		vertex,
		texture,
		maxBuffers
	};

	uint buffersId[maxBuffers];
	uint buffersLength[maxBuffers];

	uint* indices = nullptr;
	float* normals = nullptr;
	float* vertices = nullptr;
	float* texture_coord = nullptr;
};

struct TextureData
{
	const char* path;

	uint width;
	uint height;
	uint ID;
};

namespace Importer
{
	bool Init()
	{
		// Stream log messages to Debug window
		struct aiLogStream stream;
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
		aiAttachLogStream(&stream);
		return true;
	}

	bool CleanUp()
	{
		aiDetachAllLogStreams();
		return true;
	}

	void LoadMesh(char* file_path, std::vector<MeshData*> &mesh_container)
	{
		const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
		

		if (scene != nullptr && scene->HasMeshes())
		{
			// Use scene->mNumMeshes to iterate on scene->mMeshes array 
			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				MeshData* ourMesh = new MeshData();

				ourMesh->buffersId[MeshData::index] = 0;
				ourMesh->buffersId[MeshData::normal] = 0;
				ourMesh->buffersId[MeshData::vertex] = 0;

				aiMesh* loaded_mesh = scene->mMeshes[i];

				// copy vertices
				ourMesh->buffersLength[MeshData::vertex] = loaded_mesh->mNumVertices;
				ourMesh->vertices = new float[ourMesh->buffersLength[MeshData::vertex] * 3];
				memcpy(ourMesh->vertices, loaded_mesh->mVertices, sizeof(float) * ourMesh->buffersLength[MeshData::vertex] * 3);
				/*LOG("Error loading scene %s", ourMesh->num_vertices);*/

				// copy faces
				if (loaded_mesh->HasFaces())
				{
					ourMesh->buffersLength[MeshData::index] = loaded_mesh->mNumFaces * 3;
					ourMesh->indices = new uint[ourMesh->buffersLength[MeshData::index]]; // assume each face is a triangle
					for (uint j = 0; j < loaded_mesh->mNumFaces; ++j)
					{
						if (loaded_mesh->mFaces[j].mNumIndices == 3)
						{
							memcpy(&ourMesh->indices[j * 3], loaded_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
						}
					}
				}
				if (loaded_mesh->HasNormals())
				{
					ourMesh->buffersLength[MeshData::normal] = loaded_mesh->mNumVertices;
					ourMesh->normals = new float[ourMesh->buffersLength[MeshData::normal] * 3];
					memcpy(ourMesh->normals, loaded_mesh->mNormals, sizeof(float) * ourMesh->buffersLength[MeshData::normal] * 3);

				}
				mesh_container.push_back(ourMesh);
			}
			aiReleaseImport(scene);
		}
		else
		{
			LOG("Error loading scene %s", file_path);
		}
	}

	uint CreateTexture(const void* data, uint w, uint h, uint format)
	{
		uint id;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, format, w, h,
			0, format, GL_UNSIGNED_BYTE, data);

		glBindTexture(GL_TEXTURE_2D, 0);

		return id;
	}

	TextureData LoadTexture(const char* file_path)
	{
		TextureData newTexture;
		uint i;

		ilGenImages(1, &i);
		ilBindImage(i);

		if (ilLoadImage(file_path))
		{
			ILinfo ImgInfo;
			iluGetImageInfo(&ImgInfo);

			if (ImgInfo.Origin == IL_ORIGIN_UPPER_LEFT)
				iluFlipImage();

			if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
			{
				newTexture.ID = CreateTexture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT));
				newTexture.height = ilGetInteger(IL_IMAGE_HEIGHT);
				newTexture.width = ilGetInteger(IL_IMAGE_WIDTH);
				newTexture.path = file_path;
				LOG("Converted image %s", file_path);
			}
			else
			{
				LOG("Could not convert image %s", file_path);
			}
			LOG("Loaded Image %s", file_path);
		}
		else
		{
			LOG("Error loading Image %s", file_path);
		}

		return newTexture;
	}
}






