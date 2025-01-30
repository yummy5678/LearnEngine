#pragma once
#include "DescriptorBase.h"
#include "ViewProjection.h"



class VCameraDescriptor :
    public VDescriptorLayoutBase	// �x�[�X�N���X(Get�֐������܂Ƃ߂邽�߂Ɏg�p���Ă���)
{
public:
	VCameraDescriptor();
	~VCameraDescriptor();

	void Initialize(vk::Device* pLogicalDevice, uint32_t bindNumber);
	void Update(vk::Buffer buffer);

private:
	// �P��̃f�B�X�N���v�^���쐬
	void CreateDescriptorSetLayout(uint32_t bindNumber);
	void CreateDescriptorSet();
	void CreateDescriptorPool();
	void UpdateDescriptorSet(vk::DescriptorSet descriptorSet, vk::Buffer buffer, vk::DeviceSize bufferSize);



};

