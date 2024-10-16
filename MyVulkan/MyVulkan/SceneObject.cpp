#include "SceneObject.h"

SceneObject::SceneObject()
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::SetMesh(VmaAllocator allocator, MeshObject& mesh)
{
	// メッシュ情報からバッファを作成
	m_Mesh.resize(mesh.meshes.size());
	for (int i = 0; i < m_Mesh.size(); i++)
	{
		m_Mesh[i].SetMesh(allocator, mesh.meshes[i]);
	}

	// マテリアル情報からバッファを作成
	m_Material.resize(mesh.materials.size());
	for (int i = 0; i < m_Material.size(); i++)
	{
		m_Material[i].SetMaterial(allocator, mesh.materials[i]);
	}
}

std::vector<VMesh> SceneObject::GetMeshes()
{
	return m_Mesh;
}

std::vector<VMaterial> SceneObject::GetMaterials()
{
	return m_Material;
}

void SceneObject::SetTransform(glm::mat4 transform)
{
	m_Transform = transform;
}

Transform SceneObject::GetTransform()
{
	return m_Transform;
}

