#include "MeshBuffer.h"

void VMesh::SetVertex(VmaAllocator allocator, std::vector<Vertex> vertex)
{
	m_Vertex.SetData(allocator, vertex);
}

void VMesh::SetIndex(VmaAllocator allocator, std::vector<uint32_t> index)
{
	m_Index.SetData(allocator, index);
}
