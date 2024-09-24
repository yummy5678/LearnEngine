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
	// イメージのメモリ要件を取得
	vk::MemoryRequirements memoryRequirements = logicalDevice.getBufferMemoryRequirements(buffer);

	// 探しているメモリプロパティのフラグ
	vk::MemoryPropertyFlags properties = propertyFlags;
	//vk::MemoryPropertyFlags properties = vk::MemoryProperty::MemoryPropertyFlagBits::eDeviceLocalFlagBits::eHostVisible;

	// メモリの割り当て情報を設定
	vk::MemoryAllocateInfo allocateInfo;
	allocateInfo.pNext;
	allocateInfo.allocationSize = memoryRequirements.size;  // 画像のメモリサイズ
	allocateInfo.memoryTypeIndex = FindMemoryType(logicalDevice, physicalDevice, buffer, properties);   // メモリタイプ

	return allocateInfo;
}
