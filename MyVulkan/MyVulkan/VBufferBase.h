#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include <cstring>
#include "NonCopyable.h"

// バッファ作成用の基底クラス
// 頂点バッファクラスなどに派生させて使う予定


// GPU読み取り用のフラグ
//constexpr vk::BufferUsageFlags localUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

class VBufferBase : public NonCopyable
{
public:
	VBufferBase(vk::BufferUsageFlags bufferusage,	// バッファの使用用途
		VkMemoryPropertyFlags requiredFlag,			// 使用するバッファの必須要件
		VkMemoryPropertyFlags preferredFlag,		// 使用するバッファの優先要件
		VmaAllocationCreateFlags allocationFlag);	// メモリの割り当て方式
	~VBufferBase();

	virtual vk::Buffer		GetBuffer();
	virtual vk::DeviceSize	GetDataSize();

	// 作成時に登録したアロケーターを返す
	virtual const VmaAllocator* GetUsingAllocator();
	
	virtual void Cleanup();

protected:
	// 結びつける先のデバイス
	VmaAllocator*				m_pAllocator;

	// バッファの使用用途
	vk::BufferUsageFlags		m_BufferUsage;
	// キュー間の読み取り設定
	vk::SharingMode				m_SharingMode = vk::SharingMode::eExclusive;

	//VmaMemoryUsage				m_MemoryUsage = VMA_MEMORY_USAGE_AUTO;
	VmaAllocationCreateFlags	m_AllocationFlag;	// メモリの割り当て方式
	VkMemoryPropertyFlags		m_RequiredFlag;		// メモリの必須条件
	VkMemoryPropertyFlags		m_PreferredFlag;	// メモリの優先条件

	VkBuffer					m_Buffer;
	VmaAllocation				m_Allocation;

	vk::DeviceSize				m_DataSize;
	VmaAllocationInfo			m_AllocationInfo;

	void CreateBuffer(VmaAllocator* allocator, vk::DeviceSize dataSize);

	// バッファの作成関数
	VkBufferCreateInfo CreateBufferInfo(vk::DeviceSize dataSize, vk::BufferUsageFlags usage, vk::SharingMode mode);

	// メモリ割り当て関数
	uint32_t	FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags findType);

	// データをバッファに書き込む
	void MapData(void* dstData, void* setData);

};

// トランスファーバッファについて