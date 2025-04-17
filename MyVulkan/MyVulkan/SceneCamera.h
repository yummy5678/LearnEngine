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


	void SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// �f�X�N���v�^�Z�b�g��o�^
	void DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// ���L�҂����݂��邩�m�F
	//bool HasDescriptor(std::shared_ptr<vk::DescriptorSetLayout> layout);// ���L�҂����݂��邩�m�F
	void CleanupDeathOwner();// �����ɂȂ����I�[�i�[���폜
	void UpdateAll(vk::Buffer buffer, vk::DeviceSize bufferSize);

};

