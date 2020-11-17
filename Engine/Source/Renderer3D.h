#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"


#define MAX_LIGHTS 8

struct MeshData;

class Renderer3D : public Module
{
public:
	Renderer3D(bool start_enabled = true);
	~Renderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void OnResize(int width, int height);

	void MeshBuffer(MeshData* currentmesh);
	void ImportMesh(char* file_path);
	void DrawNormalDir(MeshData* currentmesh);
	void DrawMesh(MeshData* mymesh);
	void DropMesh();
	void CreateDirectCube();
	void CreateCheckerTexture();

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	//cube
	uint indices[36] = {
0, 1, 2,
2, 3, 0,
4, 5, 6,
6, 7, 4,
8, 9,10,
10,11, 8,
12,13,14,
14,15,12,
16,17,18,
18,19,16,
20,21,22,
22,23,20
	};
	float vertices[72] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front face)
							1, 1, 1,   1,-1, 1,   1,-1,-1,   1, 1,-1,   // v0,v3,v4,v5 (right face)
							1, 1, 1,   1, 1,-1,  -1, 1,-1,  -1, 1, 1,   // v0,v5,v6,v1 (top face)
						   -1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1,   // v1,v6,v7,v2 (left face)
						   -1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1,   // v7,v4,v3,v2 (bottom face)
							1,-1,-1,  -1,-1,-1,  -1, 1,-1,   1, 1,-1 }; // v4,v7,v6,v5 (back face)

	//Texture
	uint checkerID = 0;
	uint houseID = 0;

	std::vector<MeshData*> mesh_container;
};