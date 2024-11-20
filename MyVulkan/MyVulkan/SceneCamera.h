#pragma once
#include "ViewProjection.h"
#include "ViewProjectionBuffer.h"

class SceneCamera
{
public:
	SceneCamera();
	~SceneCamera();


	void UpdateBuffer(VmaAllocator* allocator, ViewProjection viewProjection);
	vk::DescriptorSet GetDescriptorSet();



private:
	VViewProjectionBuffer	m_ProjectionBuffer;
	vk::DescriptorSet		m_DescriptorSet;
	vk::DescriptorPool		m_DescriptorPool;

	// 単一のディスクリプタを作成
	void CreateSingleDescriptorSet(vk::Device logicalDevice, vk::DescriptorPool descriptorPool);
	void CreateSingleDescriptorPool(vk::DescriptorPool& setPool, vk::Device logicalDevice);
	void UpdateDescriptorSet(vk::Device logicalDevice, vk::DescriptorSet descriptorSet, vk::Buffer buffer, vk::DeviceSize bufferSize);



};

