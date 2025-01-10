#pragma once
#include "DescriptorBase.h"
#include "ViewProjection.h"



class VCameraDescriptor :
    public VDescriptorBase
{
public:
	VCameraDescriptor();
	~VCameraDescriptor();

	void Initialize(vk::Device* pLogicalDevice);
	void Update(vk::Buffer buffer);

private:
	// 単一のディスクリプタを作成
	void CreateDescriptorSetLayout(vk::Device* pLogicalDevice);
	void CreateDescriptorSet(vk::Device* pLogicalDevice);
	void CreateDescriptorPool(vk::Device* pLogicalDevice);
	void UpdateDescriptorSet(vk::DescriptorSet descriptorSet, vk::Buffer buffer, vk::DeviceSize bufferSize);



};

