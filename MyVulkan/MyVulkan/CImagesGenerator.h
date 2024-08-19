#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"

struct SwapchainImage
{
	vk::Image		image;		// VulkanÇÃíÜÇ≈àµÇ§âÊëú
	vk::ImageView	imageView;	// âÊëúÉrÉÖÅ[
};

class CImagesGenerator : public CGeneratorBase
{
public:
	CImagesGenerator();
	~CImagesGenerator();

	void Create(uint32_t ImageNum, vk::Extent3D extent, vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);
	void Destroy();


	std::vector<vk::Image>		GetSwapChainImages();
	std::vector<vk::ImageView>	GetSwapChainImageViews();




private:
	vk::Device m_LogicalDevice;
	std::vector<vk::Image> m_Images;
	std::vector<vk::ImageView> m_ImageViews;
	vk::DeviceMemory m_ImageMemory;

	vk::ImageCreateInfo CreateImageInfo(vk::Extent3D extent);
	vk::ImageViewCreateInfo CreateImageViewInfo(vk::Image image);

	//	à»â∫ÇÊÇ≠ï™ÇÁÇÒÅBå„Ç≈í≤Ç◊ÇÈ
	vk::DeviceMemory AllocateImageMemory(vk::Device device, vk::PhysicalDevice physicalDevice, vk::Image image);
	uint32_t FindMemoryType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties);

};

