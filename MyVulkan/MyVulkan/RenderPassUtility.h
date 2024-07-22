#pragma once
#include <vulkan/vulkan.hpp>
#include "SwapChainUtility.h"

class RenderpassGenerator
{
public:
	RenderpassGenerator(vk::Device logicalDevice, vk::SwapchainCreateInfoKHR* swapchainInfo);
	~RenderpassGenerator();

	[[nodiscard]]vk::RenderPass		GetRenderpass();
	void Release();

private:
	vk::AttachmentDescription				m_ColorAttachment;
	vk::AttachmentReference					m_ColorAttachmentRef;
	vk::SubpassDescription					m_Subpass;
	std::vector<vk::SubpassDependency>		m_Dependencies;
	vk::RenderPassCreateInfo				m_RenderPassInfo;
	vk::UniqueRenderPass					m_RenderPass;

	void CreateRenderpass(vk::Device logicalDevice, vk::SwapchainCreateInfoKHR* swapchainInfo);
	vk::AttachmentDescription*				CreateColorAttachment(const vk::SwapchainCreateInfoKHR* swapchainInfo);
	vk::SubpassDescription*					CreateSubpass();
	std::vector<vk::SubpassDependency>*		CreateDependencies();

	vk::RenderPassCreateInfo				CreateInfo(vk::AttachmentDescription* colorAttachment, vk::SubpassDescription* subpass, std::vector<vk::SubpassDependency>* dependencies);






};