#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "QueueUtility.h"

namespace CommandUtility
{
	vk::UniqueCommandPool createCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	std::vector<vk::CommandBuffer> createCommandBuffers(vk::Device logicalDevice, std::vector<vk::Framebuffer> framebuffers, vk::CommandPool commandPool);
	
	void recordCommands(vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline, std::vector<vk::Framebuffer> framebuffers, std::vector<vk::CommandBuffer> commandBuffers);

	void createSynchronisation();
};

class CommandGenerator : public CGeneratorBase
{
public:
	CommandGenerator();
	~CommandGenerator();

	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, std::vector<vk::Framebuffer> framebuffers);

	vk::CommandPool GetPool();
	std::vector<vk::CommandBuffer> GetBuffers();


private:
	vk::UniqueCommandPool			m_Pool;
	std::vector<vk::CommandBuffer>	m_Buffers;

	vk::CommandPool CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	std::vector<vk::CommandBuffer> CreateCommandBuffers(vk::Device logicalDevice, std::vector<vk::Framebuffer> framebuffers, vk::CommandPool commandPool);

};