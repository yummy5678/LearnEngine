#pragma once
#include "MeshObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VMesh
{
public:
	VMesh();
	~VMesh();

	void SetMesh(VmaAllocator* allocator, Mesh mesh);

	VVertexBuffer	GetVertex();
	VIndexBuffer	GetIndex();

	void Cleanup();

private:
	VVertexBuffer	m_Vertex;
	VIndexBuffer	m_Index;	

	void SetVertex(VmaAllocator* allocator, std::vector<Vertex>  vertex);
	void SetIndex(VmaAllocator* allocator, std::vector<uint32_t> index);


};

