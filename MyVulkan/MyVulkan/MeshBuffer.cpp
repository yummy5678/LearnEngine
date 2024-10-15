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

void VMesh::SetVertex(VmaAllocator allocator, std::vector<Vertex> vertex)
{
	m_Vertex.resize(vertex.size());
	for (int i = 0; i < vertex.size(); i++)
	{
		m_Vertex[i].SetData(allocator, vertex);
	}

}

void VMesh::SetIndex(VmaAllocator allocator, std::vector<uint32_t> index)
{
	m_Index.resize(index.size());
	for (int i = 0; i < index.size(); i++)
	{
		m_Index[i].SetData(allocator, index);
	}

}
