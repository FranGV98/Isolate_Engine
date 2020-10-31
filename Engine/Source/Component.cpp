#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* root, ComponentClass type, bool active) : root(root), type(type), active(active)
{

}

Component::~Component()
{

}

void Component::Update(float dt)
{

}

void Component::Enable()
{
	active = true;
}

void Component::Disable()
{
	active = false;
}

bool Component::IsActive()
{
	return active;
}

ComponentClass Component::GetType()
{
	switch (type)
	{
	case ComponentClass::TRANSFORM:
		return ComponentClass::TRANSFORM;

	case ComponentClass::MESH:
		return ComponentClass::MESH;

	case ComponentClass::NONE:
		return ComponentClass::NONE;
	}
}

GameObject* Component::GetRoot()
{
	return root;
}