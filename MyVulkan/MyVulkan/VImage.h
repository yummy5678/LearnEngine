#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>

class VImage
{
public:
	VImage();
	~VImage();

	void Create(VmaAllocator* allocator, vk::ImageCreateInfo createInfo, vk::ImageAspectFlags aspectFlag);
	void Cleanup();

	vk::Image		GetImage();
	vk::ImageView	GetImageView();

private:
	VmaAllocator* m_Allocator;

	vk::Image				m_Image;			// イメージバッファ
	vk::ImageCreateInfo		m_ImageCreateInfo;	// イメージ作成情報
	vk::ImageView			m_ImageView;		// イメージビュー
	VmaAllocation			m_ImageAllocation;	// データバッファの領域


	void CreateBuffer(VmaAllocator* allocator, vk::ImageCreateInfo createInfo);
	void CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag);

};

