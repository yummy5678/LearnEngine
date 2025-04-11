#include "MeshBuffer.h"

VMesh::VMesh()
{
}

VMesh::~VMesh()
{
	printf("メッシュのバッファをを解放します");
	Cleanup();
}

void VMesh::SetMesh(VmaAllocator* allocator, Mesh* mesh, vk::Fence fence)
{
	SetVertex(allocator, &mesh->vertices, fence);
	SetIndex(allocator, &mesh->indices, fence);
}

VVertexBuffer* VMesh::GetPVertex()
{
	return &m_Vertex;
}

VIndexBuffer* VMesh::GetPIndex()
{
	return &m_Index;
}

void VMesh::Cleanup()
{
	m_Vertex.Cleanup();
	m_Index.Cleanup();
}

void VMesh::SetVertex(VmaAllocator *allocator, std::vector<Vertex>* vertex, vk::Fence fence)
{
	m_Vertex.SetData(allocator, vertex, fence);
}

void VMesh::SetIndex(VmaAllocator* allocator, std::vector<uint32_t>* index, vk::Fence fence)
{
	m_Index.SetData(allocator, index, fence);

}

