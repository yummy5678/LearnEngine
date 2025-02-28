#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include "VImage.h"
#include "ImageSet.h"

constexpr vk::Format DefaultImageFormat = vk::Format::eR8G8B8A8Unorm;

class SwapChainImage 
{
public:
	SwapChainImage();
	~SwapChainImage();
	void Create(VmaAllocator* allocator, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo);
	void Destroy();

	//std::vector<vk::Image>		GetColorImages();
	vk::Format					GetColorFormat();
	//std::vector<vk::ImageView>	GetColorImageViews();

	//std::vector<VImage>			GetDepthImages();
	vk::Format					GetDepthFormat();

	std::vector<RenderingImageSet>	GetImageSets();
private:
	VmaAllocator* m_pAllocator;
	//vk::SwapchainKHR				m_Swapchain;
	uint32_t m_Size;

	std::vector<RenderingImageSet>	m_ImageSets;
	std::vector<VmaAllocation>		m_DepthImageAllocation;

	// カラーイメージ
	//std::vector<vk::Image>			m_ColorImages;
	vk::Format						m_ColorFormat;
	/*std::vector<vk::ImageView>		m_ColorImageViews;*/
	//VTextureDescriptorSetLayout				m_ColorDescriptor;

	// 深度イメージ
	//std::vector<VImage>				m_DepthImages;
	vk::Format						m_DepthFormat;
	//VTextureDescriptorSetLayout				m_DepthDescriptor;

	void CreateImageSet(vk::Device logicalDevice, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo);

	//void CreateColor(vk::ImageView& setView, vk::SwapchainCreateInfoKHR m_SwapchainInfo);
	//void CreateDepth(VmaAllocator* allocator, vk::SwapchainCreateInfoKHR m_SwapchainInfo);
	void CreateDepthImage(vk::Image& setImage, VmaAllocation& allocation, VmaAllocator* allocator, vk::ImageCreateInfo createInfo);


	vk::ImageCreateInfo CreateImageInfo(vk::Extent2D extent, vk::Format fomat, vk::ImageUsageFlags usage);
	vk::ImageViewCreateInfo CreateImageViewInfo(vk::Image image, vk::Format fomat, vk::ImageAspectFlags aspectFlag);


	vk::MemoryAllocateInfo AllocateImageMemory(vk::Device device, vk::PhysicalDevice physicalDevice,vk::Image image, vk::MemoryPropertyFlags propertyFlags);
	uint32_t FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Image image, vk::MemoryPropertyFlags findType);

};

// イメージ画像について