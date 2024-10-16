#pragma once
#include <vulkan/vulkan.hpp>

// ディスクリプタとは
// シェーダーのプログラムがリソースを
// 読めるようにするためのオブジェクト

class VDescriptorBase
{
public:
	VDescriptorBase();
	~VDescriptorBase();

	void SetData(vk::Device device, vk::DescriptorType descriptorType, vk::ShaderStageFlags stageFlags);
	void CreateDescriptorPool(uint32_t descriptorCount);
	vk::DescriptorSet CreateDescriptorSet(vk::Buffer buffer, vk::DeviceSize bufferSize);
	vk::DescriptorSetLayout GetDescriptorSetLayout();




protected:
	vk::Device m_Device = nullptr;
	vk::DescriptorType m_DescriptorType;
	vk::ShaderStageFlags m_StageFlags;
	vk::DescriptorPool m_DescriptorPool = nullptr;
	vk::DescriptorSetLayout m_DescriptorSetLayout = nullptr;


	void CreateDescriptorSetLayout();




};

