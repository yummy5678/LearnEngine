#pragma once
#include "NonCopyable.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VMesh : public NonCopyable
{
public:
	VMesh();
	~VMesh();

	void SetMesh(VmaAllocator* allocator, Mesh* mesh, vk::Fence fence);

	VVertexBuffer*	GetPVertex();
	VIndexBuffer*	GetPIndex();

	void Cleanup();

private:
	VVertexBuffer	m_Vertex;
	VIndexBuffer	m_Index;	

	void SetVertex(VmaAllocator* allocator, std::vector<Vertex>*  vertex, vk::Fence fence);
	void SetIndex(VmaAllocator* allocator, std::vector<uint32_t>* index, vk::Fence fence);

};

