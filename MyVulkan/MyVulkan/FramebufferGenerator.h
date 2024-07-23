#pragma once
#include <vulkan/vulkan.hpp>
#include "SwapChainUtility.h"

class FramebufferGenerator
{
public:
	FramebufferGenerator(vk::Device logicalDevice, SwapchainGenerator& swapchain, vk::RenderPass renderPass);
	FramebufferGenerator(vk::Device logicalDevice, std::vector<SwapchainImage> swapChainImages, vk::RenderPass renderPass, vk::Extent2D extent);
	~FramebufferGenerator();

	std::vector<vk::Framebuffer>	GetFramebuffers();
private:
	std::vector<vk::Framebuffer>	m_Framebuffers;

	void CreateFramebuffers(vk::Device logicalDevice, std::vector<SwapchainImage> swapChainImages, 
		vk::RenderPass renderPass, vk::Extent2D extent);

};

