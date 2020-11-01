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

	// Get/Set
	const char* GetName() const { return name.c_str(); }
	void SetName(const char* new_name);
	bool isActive() const { return active; }
	void SetActive(bool toggle_active);
	
	//Used for the hierarchy tree
	GameObject* parent;
	std::vector<GameObject*> children;

	//Components
	std::vector<Component*> components;
	ComponentTransform* transform = nullptr;

	//Children
	void InsertChild(GameObject* new_child);
	void RemoveChild(GameObject* to_erase_child);
	void UpdateActiveChildren(bool toggle_active, GameObject* parent);
	void UpdateChildren(float dt);
	
	//Components
	void UpdateComponents(float dt);
	void AddComponent(ComponentClass type);
	bool HasComponent(ComponentClass type);

private:

	std::string name;
	bool active;


};
