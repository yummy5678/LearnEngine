#pragma once
#include <vulkan/vulkan.hpp>
#include "SwapChainUtility.h"

namespace VulkanCreate
{
	[[nodiscard]] vk::RenderPassCreateInfo GetRenderPassInfo(vk::SwapchainCreateInfoKHR* swapchainInfo);

};

struct RenderPassGenerator
{
public:
	RenderPassGenerator();
	~RenderPassGenerator();



private:
	vk::AttachmentDescription	m_ColorAttachment;
	vk::AttachmentReference		m_ColorAttachmentRef;
	vk::SubpassDescription		m_Subpass;
	std::array<vk::SubpassDependency, 2> m_Dependencies;
	vk::RenderPassCreateInfo m_RenderPassInfo;


};