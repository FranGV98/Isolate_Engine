#include "Application.h"
#include "Module3DImport.h"
#include "glew/include/glew.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"
#include"Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

bool Module3DImport::Init()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return true;
}

bool Module3DImport::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

MeshData* Module3DImport::LoadMesh(char* file_path)
{
	MeshData* ourMesh = new MeshData();
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMesges array 
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* loaded_mesh = scene->mMeshes[i];
			// copy vertices
			ourMesh->num_vertex = loaded_mesh->mNumVertices;
			ourMesh->vertex = new float[ourMesh->num_vertex * 3];
			memcpy(ourMesh->vertex, loaded_mesh->mVertices, sizeof(float) * ourMesh->num_vertex * 3);
			/*LOG("Error loading scene %s", ourMesh->num_vertices);*/
			
			// copy faces
			if (loaded_mesh->HasFaces())
			{
				ourMesh->num_index = loaded_mesh->mNumFaces * 3;
				ourMesh->index = new uint[ourMesh->num_index]; // assume each face is a triangle
				for (uint j = 0; j < loaded_mesh->mNumFaces; ++j)
				{
					if (loaded_mesh->mFaces[j].mNumIndices == 3)
					{
						memcpy(&ourMesh->index[j * 3], loaded_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
			}
			if (loaded_mesh->HasNormals())
			{
				ourMesh->num_normals = loaded_mesh->mNumVertices;
				ourMesh->normals = new float[ourMesh->num_normals * 3];
				memcpy(ourMesh->normals, loaded_mesh->mNormals, sizeof(float) * ourMesh->num_normals * 3);

			}
		}
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", file_path);

	return ourMesh;
}

//SHAPES
void Module3DImport::CreateDirectCube()
{
	glLineWidth(2.0f);

	glBegin(GL_TRIANGLES);
	glVertex3d(0, 0, 1);//A
	glVertex3d(1, 0, 1);//B
	glVertex3d(0, 1, 1);//C

	glVertex3d(0, 1, 1);//C
	glVertex3d(1, 0, 1);//B
	glVertex3d(1, 1, 1);//D

	glVertex3d(1, 0, 1);//B
	glVertex3d(1, 0, 0);//F
	glVertex3d(1, 1, 1);//D

	glVertex3d(1, 0, 0);//F
	glVertex3d(1, 1, 0);//H
	glVertex3d(1, 1, 1);//D

	glVertex3d(0, 1, 0);//G
	glVertex3d(0, 1, 1);//C
	glVertex3d(1, 1, 1);//D

	glVertex3d(1, 1, 1);//D
	glVertex3d(1, 1, 0);//H
	glVertex3d(0, 1, 0);//G

	glVertex3d(0, 0, 0);//E
	glVertex3d(0, 0, 1);//A
	glVertex3d(0, 1, 0);//G

	glVertex3d(0, 0, 1);//A
	glVertex3d(0, 1, 1);//C
	glVertex3d(0, 1, 0);//G

	glVertex3d(1, 0, 0);//F
	glVertex3d(0, 0, 0);//E
	glVertex3d(1, 1, 0);//H

	glVertex3d(0, 0, 0);//E
	glVertex3d(0, 1, 0);//G
	glVertex3d(1, 1, 0);//H

	glVertex3d(0, 0, 1);//A
	glVertex3d(0, 0, 0);//E
	glVertex3d(1, 0, 0);//F

	glVertex3d(1, 0, 1);//B
	glVertex3d(0, 0, 1);//A
	glVertex3d(1, 0, 0);//F
	glEnd();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Module3DImport::CreateArrayCube()
{
	uint my_id = 0;
	glGenBuffers(1, (GLuint*) & (my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data) * 3, g_vertex_buffer_data, GL_STATIC_DRAW);


	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data));
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Module3DImport::CreateIndexArrayCube()
{
	uint my_indices = 0;
	glGenBuffers(1, (GLuint*) & (my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}


