#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include "MeshObject.h"
#include "StagingImageBuffer.h"

// RAMに置かれている画像データをVRAMに移行するクラス
class VImageBuffer
{
public:
	VImageBuffer();
	~VImageBuffer();
	void Cleanup();

	void SetImage(VmaAllocator allocator, Texture& texture, vk::Format format);

	vk::Image GetImage();

protected:
	// バッファのタイプ
	vk::ImageUsageFlags		m_Usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled; // 転送先およびサンプリングに使用
	// キュー間の読み取り設定
	vk::SharingMode			m_SharingMode = vk::SharingMode::eExclusive;
	vk::Format				m_Format = vk::Format::eR8G8B8A8Unorm;

	// 結びつける先のデバイス
	VmaAllocator			m_Allocator;

	vk::Image				m_Buffer;
	VmaAllocation			m_ImageAllocation;

	VkImageCreateInfo CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight, vk::Format format, vk::ImageUsageFlags usage, vk::SharingMode mode);
	void CreateBuffer(VmaAllocator allocator, uint32_t imageWidth, uint32_t imageHeight);
};

