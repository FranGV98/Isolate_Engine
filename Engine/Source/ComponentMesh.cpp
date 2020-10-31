#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"


ComponentMesh::ComponentMesh(GameObject* root) : Component(root, ComponentClass::MESH, true)
{

}

ComponentMesh::~ComponentMesh()
{

}

void ComponentMesh::Update(float dt)
{

}

void ComponentMesh::DrawMesh()
{
	//App->renderer3D->DrawMesh(&mesh, root->transform->GetTransform());
}