#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"


#define MAX_LIGHTS 8

struct MeshData;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

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

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	std::vector<MeshData*> mesh_container;
};