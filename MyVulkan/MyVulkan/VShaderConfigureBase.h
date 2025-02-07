#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "Utilities.h"


// �V�F�[�_�[�Ǘ��̊��N���X

class VShaderConfigureBase
{
public:
	//�R���X�g���N�^(�e�b�Z�[�V�����V�F�[�_�[�͖��Ή�)
	VShaderConfigureBase();
	~VShaderConfigureBase();
	
	virtual std::vector<vk::PipelineShaderStageCreateInfo>	GetShaderStages();
	//virtual std::vector<vk::DescriptorSet>					GetDescriptorSets() = 0;
	virtual std::vector<vk::DescriptorSetLayout>			GetDescriptorSetLayouts() = 0;
	virtual std::vector<vk::PushConstantRange>				GetPushConstantRanges() = 0;
	virtual vk::PipelineVertexInputStateCreateInfo			GetVertexInputState() = 0;

	virtual void DestroyModule();

protected:
	vk::Device* m_pLogicalDevice;

	// �V�F�[�_�[�p�X
	std::string m_VertexShaderPath;	// ���_�V�F�[�_�[�̃p�X
	std::string m_FragmentShaderPath; // �s�N�Z���V�F�[�_�[�̃p�X
	std::string m_VertexEntryName;	// ���_�V�F�[�_�[�̃G���g���[�|�C���g��
	std::string m_FragmentEntryName; // �s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g��

	// �V�F�[�_�[���W���[��
	vk::ShaderModule			m_VertexShaderModule;
	vk::ShaderModule			m_FragmentShaderModule;

	// �쐬���
	vk::PipelineShaderStageCreateInfo m_VertexStageInfo;
	vk::PipelineShaderStageCreateInfo m_FragmentStageInfo;

	void CreateShaderModules(
		vk::Device* pLogicalDevice, 
		const char* vertexPath, 
		const char* flagmentPath, 
		const char* vertexEntry, 
		const char* fragmentEntry);	// �쐬�֐�
private:
	vk::ShaderModule CreateShaderModule(const std::vector<char>& code);
	vk::PipelineShaderStageCreateInfo CreateShaderStage(vk::ShaderModule module, vk::ShaderStageFlagBits type, const char* shaderPath);

};

