#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include "MeshObject.h"
#include "StagingImageBuffer.h"

// RAMに置かれている画像データをVRAMに移行するクラス
class VTextureBuffer
{
public:
	VTextureBuffer();
	~VTextureBuffer();
	void Cleanup();

	void SetImage(VmaAllocator* allocator, Texture& texture);

	vk::Image GetImageBuffer();

	vk::ImageView GetImageView();

protected:
	VmaAllocator*			m_Allocator;


	// バッファのタイプ
	vk::ImageUsageFlags		m_Usage;
	// キュー間の読み取り設定
	vk::SharingMode			m_SharingMode;
	vk::Format				m_Format;
	vk::ImageAspectFlags	m_AspectFlag;

	vk::Image				m_Buffer;			// イメージバッファ
	vk::ImageView			m_ImageView;		// イメージビュー
	VmaAllocation			m_ImageAllocation;	// データバッファの領域	

	VmaMemoryUsage				m_MemoryUsage;
	VmaAllocationCreateFlagBits m_AllocationFlag;

	VkImageCreateInfo CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight, vk::Format format, vk::ImageUsageFlags usage, vk::SharingMode mode);
	void CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight);
	void CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag);
};

