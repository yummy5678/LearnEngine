#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"

constexpr vk::Format DefaultImageFormat = vk::Format::eR8G8B8A8Unorm;

class ImagesGenerator : public CGeneratorBase
{
public:
	ImagesGenerator();
	~ImagesGenerator();

	void Create(uint32_t ImageNum, vk::Extent2D extent, vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);
	void CreateForSwapchain(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo);
	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t imageCount, vk::Format fomat, vk::Extent2D extent);
	void Destroy();

	std::vector<vk::Image>		GetImages();
	std::vector<vk::ImageView>	GetImageViews();
	std::vector<void*>			GetImageData();
	vk::ImageCreateInfo			GetImageInfo();
	vk::Format					GetFomat();
	uint32_t					GetSize();

private:
	vk::Device m_LogicalDevice;
	uint32_t m_Size;
	std::vector<vk::Image>			m_Images;
	std::vector<vk::ImageView>		m_ImageViews;
	std::vector<vk::DeviceMemory>	m_ImageMemory;

	std::vector<void*>				m_ImageData;	//RAMに置く画像データのポインタ

	vk::Format m_Fomat = DefaultImageFormat;

	vk::ImageCreateInfo m_ImageInfo;

	vk::ImageCreateInfo CreateImageInfo(vk::Extent2D extent, vk::Format fomat);
	vk::ImageCreateInfo CreateImageInfo(vk::Extent2D extent, vk::Format fomat, vk::ImageLayout layout);
	vk::ImageViewCreateInfo CreateImageViewInfo(vk::Image image, vk::Format fomat);


	vk::MemoryAllocateInfo AllocateImageMemory(vk::Image image, vk::Device device, vk::PhysicalDevice physicalDevice);
	uint32_t FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Image image, vk::MemoryPropertyFlags findType);

};

// イメージ画像について