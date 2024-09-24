#include "BufferGenerator.h"

BufferGenerator::BufferGenerator()
{
}

BufferGenerator::~BufferGenerator()
{
}

void BufferGenerator::CreateVertex(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t vertexCount)
{
	m_bCreated = true;

	m_LogicalDevice = logicalDevice;
	m_PhysicalDevice = physicalDevice;

	m_BufferCount = vertexCount;

	for (int i = 0; i < m_BufferCount; i++)
	{

	}
	
}

void BufferGenerator::Destroy()
{
}

vk::BufferCreateInfo BufferGenerator::CreateVertexBufferInfo(uint32_t vertexSize, vk::Format fomat, vk::BufferUsageFlags usage)
{
	vk::BufferCreateInfo bufferCreateInfo;
	bufferCreateInfo.pNext;
	bufferCreateInfo.flags;
	bufferCreateInfo.size = vertexSize;
	bufferCreateInfo.usage = usage;
	bufferCreateInfo.sharingMode;
	bufferCreateInfo.queueFamilyIndexCount;
	bufferCreateInfo.pQueueFamilyIndices;
	return bufferCreateInfo;
}

vk::MemoryAllocateInfo BufferGenerator::AllocateBufferMemory(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags propertyFlags)
{
	// �C���[�W�̃������v�����擾
	vk::MemoryRequirements memoryRequirements = logicalDevice.getBufferMemoryRequirements(buffer);

	// �T���Ă��郁�����v���p�e�B�̃t���O
	vk::MemoryPropertyFlags properties = propertyFlags;
	//vk::MemoryPropertyFlags properties = vk::MemoryProperty::MemoryPropertyFlagBits::eDeviceLocalFlagBits::eHostVisible;

	// �������̊��蓖�ď���ݒ�
	vk::MemoryAllocateInfo allocateInfo;
	allocateInfo.pNext;
	allocateInfo.allocationSize = memoryRequirements.size;  // �摜�̃������T�C�Y
	allocateInfo.memoryTypeIndex = FindMemoryType(logicalDevice, physicalDevice, buffer, properties);   // �������^�C�v

	return allocateInfo;
}
