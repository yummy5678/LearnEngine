#include "RenderObject.h"

RenderObject::RenderObject() : 
	m_Transform(glm::mat4(1.0f)),	
	m_Mesh()

{
	// コンストラクタの実装
}

RenderObject::~RenderObject() {
	// デストラクタの実装
}

void RenderObject::SetMesh(VmaAllocator* allocator, std::shared_ptr<VMeshObject> mesh) 
{
	m_Mesh = mesh;
}

std::vector<VMesh>& RenderObject::GetMeshes()
{
	return m_Mesh->GetMeshes();
}

std::vector<VTextureBuffer>& RenderObject::GetMaterials() 
{
	return m_Mesh->GetTexture();
}

void RenderObject::SetTransform(glm::mat4 transform)
{
	m_Transform = transform;
}

Transform* RenderObject::GetPTransform() {
	return &m_Transform;
}
