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

	// �������[�t���O(������)
	// HostVisible  : CPU��������ǂݏ����o����
	// HostCoherent : �����I�ɓ������Ȃ��Ă�GPU���猩���
	vk::MemoryPropertyFlags memoryType = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;


	m_Buffer = logicalDevice.createBuffer(CreateBufferInfo(logicalDevice, sizeof(Vertex) * vertices.size()));
	m_BufferMemory = logicalDevice.allocateMemory(AllocateBufferMemoryInfo(logicalDevice, physicalDevice, m_Buffer, memoryType));


}
