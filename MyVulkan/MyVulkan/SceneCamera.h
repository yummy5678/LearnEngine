#pragma once
#include "NonCopyable.h"
#include "ViewProjection.h"
#include "ViewProjectionBuffer.h"
#include "SingleBufferDescriptor.h"


class SceneCamera : public NonCopyable
{
public:
	SceneCamera(VmaAllocator* allocator);
	~SceneCamera();

	void Cleanup();

	void UpdateBuffer(ViewProjection viewProjection);
	vk::Buffer GetProjectionBuffer();

	vk::DescriptorSet GetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);


private:
	VmaAllocator*	m_pAllocator;
	vk::Device		m_LogicalDevice;
	VViewProjectionBuffer	m_ProjectionBuffer;

	std::unordered_map<std::shared_ptr<vk::DescriptorSetLayout>, VSingleBufferDescriptor*>	m_Descriptors;

	void Initialize();


	void SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// デスクリプタセットを登録
	void DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// 所有者が存在するか確認
	//bool HasDescriptor(std::shared_ptr<vk::DescriptorSetLayout> layout);// 所有者が存在するか確認
	void CleanupDeathOwner();// 無効になったオーナーを削除
	void UpdateAll(vk::Buffer buffer, vk::DeviceSize bufferSize);

};

