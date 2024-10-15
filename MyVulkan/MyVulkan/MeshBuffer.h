#pragma once
#include "MeshObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VMesh
{
public:
	VMesh();
	~VMesh();

	void SetMesh(VmaAllocator allocator, Mesh mesh);



private:
	std::vector<VVertexBuffer>	m_Vertex;
	std::vector<VIndexBuffer>	m_Index;

	

	void SetVertex(VmaAllocator allocator, std::vector<Vertex>  vertex);
	void SetIndex(VmaAllocator allocator, std::vector<uint32_t> index);

};

