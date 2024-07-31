#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "SwapChainUtility.h"

class RenderpassGenerator : public CGeneratorBase
{
public:
	RenderpassGenerator();
	~RenderpassGenerator();

	void Create(vk::Device logicalDevice, const vk::SurfaceFormatKHR imageFormat);
	void Destroy(vk::Device logicalDevice);

	vk::RenderPass							GetRenderpass();


private:
	vk::Device								m_LogicalDevice;
	vk::AttachmentDescription				m_ColorAttachment;
	vk::AttachmentReference					m_ColorAttachmentRef;
	vk::SubpassDescription					m_Subpass;
	std::vector<vk::SubpassDependency>		m_Dependencies;
	vk::RenderPassCreateInfo				m_RenderPassInfo;
	vk::RenderPass							m_RenderPass;

	vk::RenderPass							CreateRenderpass(vk::Device logicalDevice, const vk::SurfaceFormatKHR imageFormat);
	vk::AttachmentDescription*				CreateColorAttachment(const vk::SurfaceFormatKHR imageFormat);
	vk::SubpassDescription*					CreateSubpass();
	std::vector<vk::SubpassDependency>*		CreateDependencies();

	vk::RenderPassCreateInfo				CreateInfo(vk::AttachmentDescription* colorAttachment, vk::SubpassDescription* subpass, std::vector<vk::SubpassDependency>* dependencies);






};