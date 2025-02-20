#pragma once
#include <vulkan/vulkan.hpp>
#include "DescriptorBase.h"

class VSingleTextureDescriptor : public VDescriptorBase
{
public:
	VSingleTextureDescriptor();
	~VSingleTextureDescriptor();

	void Initialize(vk::Device logicalDevice, vk::DescriptorSetLayout layout);
	void Update(vk::ImageView imageView, vk::Sampler sampler);


	vk::DescriptorSet		GetDescriptorSet();


private:

	void UpdateDescriptorSet(vk::ImageView imageView, vk::Sampler sampler);

};

