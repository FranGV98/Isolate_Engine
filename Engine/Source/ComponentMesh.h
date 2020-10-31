#ifndef __COMPONENTMESH__
#define __COMPONENTMESH__

class Component;
struct MeshData;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* root);
	ComponentMesh(GameObject* root, char* path, MeshData* mesh);

	~ComponentMesh();

	void Update(float dt) override;

	void DrawMesh();

private:

	MeshData* mesh;

};
#endif //__COMPONENTMESH__