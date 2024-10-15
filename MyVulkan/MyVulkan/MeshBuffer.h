#pragma once
#include "MeshObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "IMageBuffer"

class VMesh
{
public:
	VMesh();
	~VMesh();


	void SetVertex(VmaAllocator allocator, std::vector<Vertex> vertex);
	void SetIndex(VmaAllocator allocator, std::vector<uint32_t> index);



private:
	VVertexBuffer	m_Vertex;
	VIndexBuffer	m_Index;

	VImageBuffer	m_Texture;




};

