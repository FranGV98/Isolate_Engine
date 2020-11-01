#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "Globals.h"

GameObject::GameObject(std::string name, GameObject* parent) : name(name), parent(parent)
{
	AddComponent(ComponentClass::TRANSFORM);
	active = true;
}

GameObject::~GameObject()
{

}

bool GameObject::Update(float dt)
{
	UpdateChildren(dt);
	UpdateComponents(dt);
	return true;
}

bool GameObject::CleanUp()
{
	//CleanUp the children tree
	while (children.size() != 0)
	{
		(*children.begin())->CleanUp();
		delete* children.begin();
		children.erase(children.begin());
	}

	//CleanUp the component vector
	while (components.size() != 0)
	{
		delete* components.begin();
		components.erase(components.begin());
	}
	return true;
}

void GameObject::SetActive(bool toggle_active)
{
	this->active = toggle_active;
	UpdateActiveChildren(toggle_active, this);
}

void GameObject::UpdateActiveChildren(bool toggle_active, GameObject* parent)
{
	if (parent != nullptr)
	{
		for (int i = 0; i < parent->children.size(); ++i)
		{
			parent->children[i]->active = toggle_active;

			UpdateActiveChildren(toggle_active, parent->children[i]);
		}
	}
}

void GameObject::UpdateChildren(float dt)
{
	std::vector<GameObject*>::iterator item;
	for (item = children.begin(); item != children.end(); item++)
	{
		GameObject* child = *item;

		child->Update(dt);
	}
}

void GameObject::UpdateComponents(float dt)
{
	std::vector<Component*>::iterator item;
	for (item = components.begin(); item != components.end(); item++)
	{
		(*item)->Update(dt);
	}
}


void GameObject::AddComponent(ComponentClass type)
{
	switch (type)
	{
	case ComponentClass::TRANSFORM:
	{
		if (!HasComponent(ComponentClass::TRANSFORM))
		{
			ComponentTransform* n_transform = new ComponentTransform(this);
			components.push_back(n_transform);
			transform = n_transform;
		}
		else 
		{
			LOG("Oops! You already have a TRANSFORM component");
		}
		break;
	}

	case ComponentClass::MESH:
	{
		if (!HasComponent(ComponentClass::MESH))
		{
			ComponentMesh* n_mesh = new ComponentMesh(this);
			components.push_back(n_mesh);
		}
		else
		{
			LOG("Oops! You already have a MESH component");
		}
		break;
	}

	case ComponentClass::NONE:
	{
		break;
	}
	}
}

bool GameObject::HasComponent(ComponentClass type)
{
	std::vector<Component*>::iterator item;
	for (item = components.begin(); item != components.end(); ++item)
	{
		if ((*item)->GetType() == type)
		{
			return true;
		}
	}
	return false;
}

void GameObject::SetName(const char* new_name)
{
	name = new_name;
}

void GameObject::InsertChild(GameObject* new_child)
{

	if (new_child->parent != nullptr)
	{
		new_child->parent->RemoveChild(new_child);
	}

	ComponentTransform* child_transform = new_child->transform;

	child_transform->UpdateTransform();

	new_child->parent = this;

	children.push_back(new_child);

}

void GameObject::RemoveChild(GameObject* child)
{
	std::vector<GameObject*>::iterator item;
	for (item = children.begin(); item != children.end(); item++)
	{
		if (*item == child)
		{
			children.erase(item);
			break;
		}
	}
}