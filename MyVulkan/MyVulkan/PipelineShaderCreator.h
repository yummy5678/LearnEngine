#pragma once
#include "VShaderConfigureBase.h"
#include "GraphicsDefine.h"
#include "MeshObject.h"
#include "VTextureDescriptor.h"
#include "VCameraDescriptor.h"

// �V�F�[�_�[�̓ǂݍ��݂ƃ��W���[���A�X�e�[�W�̍쐬���s���N���X
// �ꏏ�Ƀf�X�N���v�^�̊Ǘ����s�� 

class VModelShaderConfiguer : public VShaderConfigureBase
{
public:
	VModelShaderConfiguer();
	~VModelShaderConfiguer();


	// �쐬�֐�(�e�b�Z�[�V�����V�F�[�_�[�͖��Ή�)
	void Create(vk::Device* pLogicalDevice);

	
	//std::vector<vk::PipelineShaderStageCreateInfo> GetShaderStages();
	std::vector<vk::DescriptorSet>			GetDescriptorSets()			override;
	std::vector<vk::DescriptorSetLayout>	GetDescriptorSetLayouts()	override;
	std::vector<vk::PushConstantRange>		GetPushConstantRanges()		override;
	vk::PipelineVertexInputStateCreateInfo	GetVertexInputState()		override;

protected:

	// �f�X�N���v�^
	VTextureDescriptor		m_TextureDescriptor;
	VCameraDescriptor		m_CameraDescriptor;

	// ���_���͏��	
	std::vector<vk::VertexInputBindingDescription>		m_BindingDescriptions;
	std::vector<vk::VertexInputAttributeDescription>	m_AttributeDescriptions;

	//vk::ShaderModule CreateShaderModule(vk::Device logicalDevice, const std::vector<char>& code);
	//vk::PipelineShaderStageCreateInfo CreateShaderStage(vk::ShaderModule module, vk::ShaderStageFlagBits type);

	vk::PushConstantRange	GetPushConstantModelRange();

};

