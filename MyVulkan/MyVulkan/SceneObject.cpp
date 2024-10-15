#include "SceneObject.h"

SceneObject::SceneObject()
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::SetPMesh(MeshObject* pMesh)
{
	m_Mesh = *pMesh;

	m_VertexBuffer.CreateMeshObject();
}

MeshObject SceneObject::GetMesh()
{
	return m_Mesh;
}

vk::Buffer SceneObject::GetVertexBuffer()
{
	return m_VertexBuffer;
}
