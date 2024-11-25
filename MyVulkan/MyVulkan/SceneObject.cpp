#include "SceneObject.h"

SceneObject::~SceneObject() {
	// デストラクタの実装
}

void SceneObject::SetMesh(VmaAllocator* allocator, VMeshObject* mesh) {
		m_Mesh = mesh->GetMeshes();

		// マテリアル情報からバッファを作成
		m_Material = std::make_shared<std::vector<VMaterial>>(mesh->GetTexture()->size());
		for (size_t i = 0; i < m_Material->size(); i++)
		{
			(*m_Material)[i].SetMaterial(allocator, mesh->GetTexture()->at(i));
		}
}

std::shared_ptr<std::vector<VMesh>> SceneObject::GetMeshes() {
	return m_Mesh;
}

std::shared_ptr<std::vector<VMaterial>> SceneObject::GetMaterials() {
	return m_Material;
}

void SceneObject::SetTransform(glm::mat4 transform)
{
	m_Transform = transform;
}

Transform* SceneObject::GetPTransform() {
	return &m_Transform;
}
