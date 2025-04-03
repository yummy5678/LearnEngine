#include "MeshBuffer.h"

VMesh::VMesh()
{
}

VMesh::~VMesh()
{
	/*Cleanup();*/
}

void VMesh::SetMesh(VmaAllocator* allocator, Mesh* mesh)
{
	SetVertex(allocator, &mesh->vertices);
	SetIndex(allocator, &mesh->indices);
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

void VMesh::SetVertex(VmaAllocator *allocator, std::vector<Vertex>* vertex)
{
	m_Vertex.SetData(allocator, vertex);
}

void VMesh::SetIndex(VmaAllocator* allocator, std::vector<uint32_t>* index)
{
	m_Index.SetData(allocator, index);

}

