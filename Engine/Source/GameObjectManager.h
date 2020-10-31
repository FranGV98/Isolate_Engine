#pragma once
#include "Module.h"

class GameObject;

class GameObjectManager : public Module
{
public:

	GameObjectManager(bool is_active = true);
	virtual ~GameObjectManager();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate();
	bool CleanUp();

	GameObject* CreateGameObject(std::string name, GameObject* parent = nullptr);

	std::vector<GameObject*> game_objects;

};