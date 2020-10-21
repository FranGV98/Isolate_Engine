
#include "Globals.h"


#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"


#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "glew/libx86/glew32.lib")

struct MeshData
{
	enum Buffers
	{
		index,
		normal,
		vertex,
		maxBuffers
	};

	uint buffersId[maxBuffers];
	uint buffersLength[maxBuffers];

	uint* indices = nullptr;
	float* normals = nullptr;
	float* vertices = nullptr;
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
}






