#include "VertexBuffer.h"


VertexBuffer::VertexBuffer():
	VBufferBase(vk::BufferUsageFlagBits::eVertexBuffer)
{

}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::Create(vk::Device logicalDevice,vk::PhysicalDevice physicalDevice, std::vector<Vertex> vertices)
{

	// メモリーフラグ(メモリ)
	// HostVisible  : CPU側からも読み書き出来る
	// HostCoherent : 明示的に同期しなくてもGPUから見れる
	vk::MemoryPropertyFlags memoryType = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;


	m_Buffer = logicalDevice.createBuffer(CreateBufferInfo(logicalDevice, sizeof(Vertex) * vertices.size()));
	m_BufferMemory = logicalDevice.allocateMemory(AllocateBufferMemoryInfo(logicalDevice, physicalDevice, m_Buffer, memoryType));


}
