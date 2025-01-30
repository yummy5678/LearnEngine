#pragma once
#include "DescriptorBase.h"
#include "ViewProjection.h"



class VCameraDescriptor :
    public VDescriptorLayoutBase	// ベースクラス(Get関数等をまとめるために使用している)
{
public:
	VCameraDescriptor();
	~VCameraDescriptor();

	void Initialize(vk::Device* pLogicalDevice, uint32_t bindNumber);
	void Update(vk::Buffer buffer);

private:
	// 単一のディスクリプタを作成
	void CreateDescriptorSetLayout(uint32_t bindNumber);
	void CreateDescriptorSet();
	void CreateDescriptorPool();
	void UpdateDescriptorSet(vk::DescriptorSet descriptorSet, vk::Buffer buffer, vk::DeviceSize bufferSize);



};

