#ifndef __COMPONENT__
#define __COMPONENT__

class GameObject;

enum ComponentClass
{
	TRANSFORM,
	MESH,
	NONE
};

class Component
{
public:

	Component(GameObject* root, ComponentClass type, bool active);
	~Component();

	virtual void Update(float dt) = 0;

	void Enable();
	void Disable();

	bool IsActive();
	ComponentClass GetType(); //Returns the type of the component

	GameObject* GetRoot(); //Returns the GameObject that owns this component

public:
	bool active = true;;
	ComponentClass type = NONE;
	GameObject* root = nullptr;
};
#endif //__COMPONENT__