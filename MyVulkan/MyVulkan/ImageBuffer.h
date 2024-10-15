#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>

// RAMに置かれている画像データをVRAMに移行するクラス
class VImageBuffer
{
public:
	VImageBuffer();
	~VImageBuffer();


	void createImage(VmaAllocation& allocation, void* data, uint32_t width, uint32_t height, vk::Format format);


protected:
	// 結びつける先のデバイス
	VmaAllocator				m_Allocator;

	// バッファのタイプ
	vk::BufferUsageFlags	m_Usage;

	// キュー間の読み取り設定
	vk::SharingMode			m_SharingMode = vk::SharingMode::eExclusive;

	vk::Image       m_Buffer;
	VmaAllocation	m_Allocation;


};

