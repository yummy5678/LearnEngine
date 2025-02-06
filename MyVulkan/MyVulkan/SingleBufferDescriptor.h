#pragma once
#include "DescriptorBase.h"


class VSingleBufferDescriptor : public VDescriptorBase
{
public:
	VSingleBufferDescriptor();
	~VSingleBufferDescriptor();

	void Initialize(vk::Device* pLogicalDevice, vk::DescriptorSetLayout layout);
	void Update(vk::Buffer buffer, vk::DeviceSize bufferSize);


	vk::DescriptorSet		GetDescriptorSet();


private:

	void UpdateDescriptorSet(vk::Buffer buffer, vk::DeviceSize bufferSize);



};

