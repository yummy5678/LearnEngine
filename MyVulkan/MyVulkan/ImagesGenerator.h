#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"

constexpr vk::Format DefaultImageFormat = vk::Format::eR8G8B8A8Unorm;

class CImagesGenerator : public CGeneratorBase
{
public:
	CImagesGenerator();
	~CImagesGenerator();

	void Create(uint32_t ImageNum, vk::Extent2D extent, vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);
	void CreateForSurface(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	void Destroy();

	std::vector<vk::Image>		GetImages();
	std::vector<vk::ImageView>	GetImageViews();
	vk::ImageCreateInfo			GetImageInfo();
	vk::Format					GetFomat();
	uint32_t					GetSize();

private:
	vk::Device m_LogicalDevice;
	uint32_t m_Size;
	std::vector<vk::Image> m_Images;
	std::vector<vk::ImageView> m_ImageViews;
	std::vector<vk::DeviceMemory> m_ImageMemory;

	vk::Format m_Fomat = DefaultImageFormat;

	vk::ImageCreateInfo m_ImageInfo;

	vk::ImageCreateInfo CreateImageInfo(vk::Extent2D extent, vk::Format fomat);
	vk::ImageViewCreateInfo CreateImageViewInfo(vk::Image image, vk::Format fomat);


	vk::MemoryAllocateInfo AllocateImageMemory(vk::Image image, vk::Device device, vk::PhysicalDevice physicalDevice);
	uint32_t FindMemoryType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter);

};

// ÉCÉÅÅ[ÉWâÊëúÇ…Ç¬Ç¢Çƒ