#pragma once
#include "Globals.h"

class Component;
class ComponentTransform;
class ComponentMesh;
enum ComponentClass;

class GameObject
{
public:

	GameObject(std::string name, GameObject* parent);
	virtual ~GameObject();

	bool Update(float dt);
	bool CleanUp();

	const char* GetName() const { return name.c_str(); }
	const bool isActive() const { return active; }
	void SetActive(bool toggle_active);

	//Used for the hierarchy tree
	GameObject* parent;
	std::vector<GameObject*> children;

	//Components
	std::vector<Component*> components;
	ComponentTransform* transform = nullptr;


	void UpdateChildren(float dt);
	void UpdateComponents(float dt);
	
	void AddComponent(ComponentClass type);
	bool HasComponent(ComponentClass type);

private:

	std::string name;
	bool active;
};
