#pragma once
//#include "DescriptorBase.h"
#include "VDescriptorLayoutBase.h"
#include "ViewProjection.h"



class VCameraDescriptorSetLayout :
    public VDescriptorSetLayoutBase	// �x�[�X�N���X(Get�֐������܂Ƃ߂邽�߂Ɏg�p���Ă���)
{
public:
	VCameraDescriptorSetLayout();
	~VCameraDescriptorSetLayout();

	void Initialize(vk::Device* pLogicalDevice, uint32_t bindNumber);
	/*void Update(vk::Buffer buffer);*/

private:
	// �P��̃f�B�X�N���v�^���쐬
	void CreateDescriptorSetLayout(uint32_t bindNumber);
	//void CreateDescriptorSet();
	//void CreateDescriptorPool();
	//void UpdateDescriptorSet(vk::DescriptorSet descriptorSet, vk::Buffer buffer, vk::DeviceSize bufferSize);



};

