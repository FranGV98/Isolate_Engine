#include "GameObjectManager.h"
#include "GameObject.h"

GameObjectManager::GameObjectManager(bool start_enabled) : Module(start_enabled)
{
}

GameObjectManager::~GameObjectManager()
{
}

bool GameObjectManager::Start()
{
	//Create objects here, define parent nodes as GameObjects* so we can reference them on the childs
	GameObject* garden = CreateGameObject("Garden", nullptr);
	CreateGameObject("Potato", garden);
	CreateGameObject("Tomato", garden);
	GameObject* flock = CreateGameObject("flock", garden);
	for (int b = 0; b < 5; b++)
	{
		CreateGameObject("Bird", flock);
	}

	GameObject* House = CreateGameObject("House", nullptr);
	CreateGameObject("Fireplace", House);
	
	return true;
}

update_status GameObjectManager::Update(float dt)
{
	//Update gameObjects
	for (std::vector<GameObject*>::iterator item = game_objects.begin(); item != game_objects.end(); item++)
	{
		GameObject* game_object = *item;

		game_object->Update(dt);
	}

	return UPDATE_CONTINUE;
}

update_status GameObjectManager::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool GameObjectManager::CleanUp()
{
	while (game_objects.size() != 0)
	{
		(*game_objects.begin())->CleanUp();
		delete* game_objects.begin();
		game_objects.erase(game_objects.begin());
	}

	return true;
}

GameObject* GameObjectManager::CreateGameObject(std::string name, GameObject* parent)
{
	GameObject* n_game_object = new GameObject(name, parent);
	if (!parent)
		game_objects.push_back(n_game_object);
	else
		parent->children.push_back(n_game_object);

	return n_game_object;
}
