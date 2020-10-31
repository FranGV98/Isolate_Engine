#ifndef __COMPONENTTRANSFORM__
#define __COMPONENTTRANSFORM__

#include "MathGeoLib/src/MathGeoLib.h"
#include "glmath.h"

class Component;

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* root);
	~ComponentTransform();

	void Update(float dt) override;

	float4x4 GetTransform() const { return transform; }
	float3 GetPosition() const { return position; }
	Quat GetRotation() const { return rotation; }
	float3 GetScale() const { return scale; }

	void SetPosition(float3 position);
	void SetScale(float3 scale);
	void UpdateTransform();

private:

	float4x4 transform;
	float3 position;
	Quat rotation;
	float3 scale;
	

};
#endif //__COMPONENTTRANSFORM__