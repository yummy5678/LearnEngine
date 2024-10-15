#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>

// バッファ作成用の基底クラス
// 頂点バッファクラスなどに派生させて使う予定


// GPU読み取り用のフラグ
//constexpr vk::BufferUsageFlags localUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

class VBufferBase
{
public:
	VBufferBase(vk::BufferUsageFlags usage);
	~VBufferBase();

	void SetData(void* pData,vk::DeviceSize dataSize);
	vk::Buffer GetBuffer();
	
	void Cleanup();

protected:
	// 結びつける先のデバイス
	VmaAllocator				m_Allocator;

	// バッファのタイプ
	vk::BufferUsageFlags	m_Usage;
	// キュー間の読み取り設定
	vk::SharingMode			m_SharingMode = vk::SharingMode::eExclusive;

	VkBuffer        m_Buffer;
	VmaAllocation	m_Allocation;

	void CreateBuffer(VmaAllocator allocator, vk::DeviceSize dataSize);

	// バッファの作成関数
	VkBufferCreateInfo CreateBufferInfo(vk::DeviceSize dataSize, vk::BufferUsageFlags usage, vk::SharingMode mode);

	// メモリ割り当て関数
	uint32_t	FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags findType);



};

// トランスファーバッファについて