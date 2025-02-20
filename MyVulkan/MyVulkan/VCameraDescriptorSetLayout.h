#pragma once
//#include "DescriptorBase.h"
#include "VDescriptorLayoutBase.h"
#include "ViewProjection.h"



class VCameraDescriptorSetLayout :
    public VDescriptorSetLayoutBase	// ベースクラス(Get関数等をまとめるために使用している)
{
public:
	VCameraDescriptorSetLayout();
	~VCameraDescriptorSetLayout();

	void Initialize(vk::Device* pLogicalDevice, uint32_t bindNumber);
	/*void Update(vk::Buffer buffer);*/

private:
	// 単一のディスクリプタを作成
	void CreateDescriptorSetLayout(uint32_t bindNumber);
	//void CreateDescriptorSet();
	//void CreateDescriptorPool();
	//void UpdateDescriptorSet(vk::DescriptorSet descriptorSet, vk::Buffer buffer, vk::DeviceSize bufferSize);



};

