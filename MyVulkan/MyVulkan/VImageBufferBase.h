#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include "MeshObject.h"
#include "StagingImageBuffer.h"



class VImageBufferBase
{
public:
	VImageBufferBase(vk::ImageUsageFlags bufferusage, 
		VkMemoryPropertyFlags requiredFlag,			// 使用するバッファの必須要件
		VkMemoryPropertyFlags preferredFlag,		// 使用するバッファの優先要件
		VmaAllocationCreateFlags allocationFlag);	// メモリの割り当て方式
	~VImageBufferBase();

	//void SetImage(VmaAllocator* allocator, uint32_t textureWidth, uint32_t textureHeight);
	vk::Image GetImageBuffer();
	vk::ImageView GetImageView();

	void Cleanup();




protected:
	VmaAllocator* m_pAllocator;


	// バッファのタイプ
	vk::ImageUsageFlags		m_Usage;
	// キュー間の読み取り設定
	vk::SharingMode			m_SharingMode;
	vk::Format				m_Format;
	vk::ImageAspectFlags	m_AspectFlag;

	vk::Image				m_ImageBuffer;			// イメージバッファ
	vk::ImageView			m_ImageView;		// イメージビュー
	VmaAllocation			m_ImageAllocation;	// データバッファの領域	

	VmaAllocationCreateFlags	m_AllocationFlag;	// メモリの割り当て方式
	VkMemoryPropertyFlags		m_RequiredFlag;		// メモリの必須条件
	VkMemoryPropertyFlags		m_PreferredFlag;	// メモリの優先条件

	VkImageCreateInfo CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight);
	void CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight);
	void CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag);


	// データをバッファに書き込む
	void MapData(void* setData, vk::DeviceSize dataSize);
};

