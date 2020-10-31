
#include "Component.h"
#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject* root) : Component(root, ComponentClass::TRANSFORM, true)
{
	position = float3(0.f, 0.f, 0.f);
	rotation = Quat::identity;
	scale = float3(1.f, 1.f, 1.f);
	transform = float4x4::FromTRS(position, rotation, scale);

}

ComponentTransform::~ComponentTransform()
{

}

void ComponentTransform::Update(float dt)
{

}
void ComponentTransform::SetPosition(float3 position)
{
	this->position = position;
	UpdateTransform();
}

void ComponentTransform::SetScale(float3 scale)
{
	this->scale = scale;
	UpdateTransform();
}
//Recalculates de transform matrix
void ComponentTransform::UpdateTransform()
{
	transform = float4x4::FromTRS(position, rotation, scale);
}