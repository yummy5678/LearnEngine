#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "QueueUtility.h"
#include "Device.h"

class CommandGenerator : public CGeneratorBase
{
public:
	CommandGenerator();
	~CommandGenerator();

	void Create(vk::Device logicalDevice, vk::PhysicalDevice phygicalDevice, uint32_t commandSize);
	void Create(DeviceGenerator , std::vector<vk::Framebuffer> framebuffers);
	void Destroy(vk::Device logicalDevice);

	void RecordCommands(vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline);

	vk::CommandPool GetPool();
	std::vector<vk::CommandBuffer> GetBuffers();


private:
	vk::Device						m_LogicalDevice;
	vk::CommandPool					m_Pool;
	std::vector<vk::CommandBuffer>	m_Buffers;

	vk::CommandPool CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice phygicalDevice);
	std::vector<vk::CommandBuffer> CreateCommandBuffers(vk::Device logicalDevice, uint32_t commandSize, vk::CommandPool commandPool);

};

//
//次のオブジェクト タイプは、Vulkan コマンドに渡されたときに消費され、
//作成に使用されたオブジェクトによってそれ以上アクセスされることはありません。
//渡される API コマンドの実行中は破棄してはなりません。
//VkShaderModule
//VkPipelineCache
//VkValidationCacheEXT

//以下コマンドバッファに関するメモ
//次の Vulkan オブジェクトは、そのオブジェクトを使用するコマンド バッファが保留状態にある間は破棄してはなりません。
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