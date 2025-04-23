#include "RenderObject.h"

RenderObject::RenderObject() : 
	m_Transform(glm::mat4(1.0f)),	
	m_Meshes()

{
	// コンストラクタの実装
}

RenderObject::~RenderObject() {
	// デストラクタの実装
}

void RenderObject::SetMesh(VmaAllocator* allocator, std::vector<MeshObject>* meshes)
{
	if (allocator == nullptr || meshes->empty() == true)
	{
		// ToDo エラーメッセージを設定する
		return;
	}

	// RAMに置かれたモデルデータをVRAMに移動
	size_t size = meshes->size();
	m_Meshes.resize(size);
	
	for (size_t i = 0; i < size; i++)
	{
		m_Meshes[i].SetMeshObject(allocator, &meshes->at(i));
	}
}

std::vector<VMeshObject>* RenderObject::GetMeshes()
{
	return &m_Meshes;
}

void RenderObject::Cleanup()
{
	for (auto& mesh: m_Meshes)
	{
		mesh.Cleanup();
	}
	m_Meshes.clear();

	m_Transform = {};
}

void RenderObject::SetTransform(glm::mat4 transform)
{
	m_Transform = transform;
}

Transform* RenderObject::GetPTransform() 
{
	return &m_Transform;
}

