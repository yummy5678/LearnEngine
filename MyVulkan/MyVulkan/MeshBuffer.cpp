#include "MeshBuffer.h"

VMesh::VMesh()
{
}

VMesh::~VMesh()
{
}

void VMesh::SetMesh(VmaAllocator allocator, Mesh mesh)
{
	SetVertex(allocator, mesh.vertices);
	SetIndex(allocator, mesh.indices);
}

VVertexBuffer VMesh::GetVertex()
{
	return m_Vertex;
}

VIndexBuffer VMesh::GetIndex()
{
	return m_Index;
}


void VMesh::SetVertex(VmaAllocator allocator, std::vector<Vertex> vertex)
{
	m_Vertex.SetData(allocator, vertex);
}

void VMesh::SetIndex(VmaAllocator allocator, std::vector<uint32_t> index)
{
	m_Index.SetData(allocator, index);

}
