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
	void Destroy(vk::Device logicalDevice);

	void RecordCommands(vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline);

	vk::CommandPool GetPool();
	std::vector<vk::CommandBuffer> GetBuffers();


private:
	vk::Device						m_LogicalDevice;
	vk::CommandPool					m_Pool;
	std::vector<vk::CommandBuffer>	m_Buffers;
	std::vector<vk::Framebuffer>	m_Framebuffers;

	vk::CommandPool CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	std::vector<vk::CommandBuffer> CreateCommandBuffers(vk::Device logicalDevice, std::vector<vk::Framebuffer> framebuffers, vk::CommandPool commandPool);

};

//
//���̃I�u�W�F�N�g �^�C�v�́AVulkan �R�}���h�ɓn���ꂽ�Ƃ��ɏ����A
//�쐬�Ɏg�p���ꂽ�I�u�W�F�N�g�ɂ���Ă���ȏ�A�N�Z�X����邱�Ƃ͂���܂���B
//�n����� API �R�}���h�̎��s���͔j�����Ă͂Ȃ�܂���B
//VkShaderModule
//VkPipelineCache
//VkValidationCacheEXT

//�ȉ��R�}���h�o�b�t�@�Ɋւ��郁��
//���� Vulkan �I�u�W�F�N�g�́A���̃I�u�W�F�N�g���g�p����R�}���h �o�b�t�@���ۗ���Ԃɂ���Ԃ͔j�����Ă͂Ȃ�܂���B
//VkEvent
//VkQueryPool
//VkBuffer
//VkBufferView
//VkImage
//VkImageView
//VkPipeline
//VkSampler
//VkSamplerYcbcrConversion
//VkDescriptorPool
//VkFramebuffer
//VkRenderPass
//VkCommandBuffer
//VkCommandPool
//VkDeviceMemory
//VkDescriptorSet
//VkIndirectCommandsLayoutNV
//VkAccelerationStructureNV
//VkAccelerationStructureKHR
//VkVideoSessionKHR
//VkVideoSessionParametersKHR