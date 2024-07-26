#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "SwapChainUtility.h"

class FramebufferGenerator : public CGeneratorBase
{
public:
	FramebufferGenerator();
	~FramebufferGenerator();

	void Create(vk::Device logicalDevice, std::vector<SwapchainImage> swapChainImages, vk::RenderPass renderPass, vk::Extent2D extent);



	std::vector<vk::Framebuffer>	GetFramebuffers();
private:
	vk::Device								m_LogicalDevice;
	std::vector<vk::Framebuffer>			m_Framebuffers;
	std::vector<vk::FramebufferCreateInfo>	m_FramebufferInfos;
	std::array<vk::ImageView, 1>			m_Attachments;

	void Destroy(vk::Device logicalDevice);
	std::vector<vk::FramebufferCreateInfo> CreateFramebufferInfos(std::vector<SwapchainImage> swapChainImages, vk::RenderPass renderPass, vk::Extent2D extent);

};

