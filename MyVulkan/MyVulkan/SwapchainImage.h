#pragma once
#include <vulkan/vulkan.hpp>


constexpr vk::Format DefaultImageFormat = vk::Format::eR8G8B8A8Unorm;

class SwapChainImage 
{
public:
	SwapChainImage();
	~SwapChainImage();
	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo);
	void Destroy();

	std::vector<vk::Image>		GetColorImages();
	std::vector<vk::ImageView>	GetColorImageViews();
	uint32_t					GetSize();

private:
	vk::Device m_LogicalDevice;
	uint32_t m_Size;
	std::vector<vk::Image>			m_ColorImages;
	std::vector<vk::ImageView>		m_ColorImageViews;
	std::vector<vk::DeviceMemory>	m_ColorImageMemory;

	std::vector<vk::Image>			m_DepthImages;
	std::vector<vk::ImageView>		m_DepthImageViews;
	std::vector<vk::DeviceMemory>	m_DepthImageMemory;


	void CreateColor(vk::Device logicalDevice, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo);
	void CreateDepth(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainCreateInfoKHR m_SwapchainInfo);


	vk::ImageCreateInfo CreateImageInfo(vk::Extent2D extent, vk::Format fomat, vk::ImageUsageFlags usage);
	vk::ImageViewCreateInfo CreateImageViewInfo(vk::Image image, vk::Format fomat, vk::ImageAspectFlags aspectFlag);


	vk::MemoryAllocateInfo AllocateImageMemory(vk::Device device, vk::PhysicalDevice physicalDevice,vk::Image image, vk::MemoryPropertyFlags propertyFlags);
	uint32_t FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Image image, vk::MemoryPropertyFlags findType);

};

// ÉCÉÅÅ[ÉWâÊëúÇ…Ç¬Ç¢Çƒ