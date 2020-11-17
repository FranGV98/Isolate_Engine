#pragma once

#include "Module.h"
#include "Globals.h"

class Primitive;

class Scene : public Module
{
public:
	Scene(bool start_enabled = true);
	~Scene();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;

	bool CleanUp();

private:
	void HandleDebugInput();
	void DebugSpawnPrimitive(Primitive* p);

	std::vector<Primitive*> primitives;
};
