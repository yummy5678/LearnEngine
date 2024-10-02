#include "SceneObject.h"

SceneObject::SceneObject()
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::SetMesh(MeshObject* pMesh)
{
	m_pMesh = pMesh;

	m_VertexBuffer.CreateMeshObject();
}

MeshObject* SceneObject::GetPMesh()
{
	return nullptr;
}
