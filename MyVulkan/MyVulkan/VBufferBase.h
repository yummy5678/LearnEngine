#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>

// バッファ作成用の基底クラス
// 頂点バッファクラスなどに派生させて使う予定



// 送信用バッファに設定する予定のフラグ
constexpr vk::BufferUsageFlags stagingUsage = vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst;
// GPU読み取り用のフラグ
//constexpr vk::BufferUsageFlags localUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

class VBufferBase
{
public:
	VBufferBase(vk::BufferUsageFlags usage);
	~VBufferBase();

	void SetData(void* pData,vk::DeviceSize dataSize);
	
	void Cleanup();

protected:
	// 結びつける先のデバイス
	//vk::Device				m_LogicalDevice;
	//vk::PhysicalDevice		m_PhysicalDevice;
	VmaAllocator				m_Allocator;

	// バッファのタイプ
	vk::BufferUsageFlags	m_DataUsage;
	// キュー間の読み取り設定
	vk::SharingMode			m_SharingMode = vk::SharingMode::eExclusive;

	VkBuffer        m_DataBuffer;
	VmaAllocation	m_DataAllocation;

	VkBuffer        m_StagingBuffer;
	VmaAllocation	m_StagingAllocation;

	void CreateBuffer(VmaAllocator allocator, vk::Buffer buffer, vk::DeviceSize dataSize);

	// バッファの作成関数
	VkBufferCreateInfo CreateBufferInfo(vk::DeviceSize dataSize, vk::BufferUsageFlags usage);

	// メモリ割り当て関数
	void		AllocateBufferMemory(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags propertyFlags);
	uint32_t	FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags findType);

	// データをステージングバッファにコピー
	void MapData(VmaAllocator allocator, void* pData, vk::DeviceSize dataSize);

};

// トランスファーバッファについて