#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "Utilities.h"
#include "GraphicsDefine.h"

// �V�F�[�_�[�̓ǂݍ��݂ƃ��W���[���A�X�e�[�W�̍쐬���s���N���X
// �V�F�[�_�[�̍쐬���ɐ����͂Ȃ����p�C�v���C�����ɂ�����
// �V�F�[�_�[��ʂ����Ԃ�VulkanAPI�̒��Ŏ����Ő��������̂��낤���H
// 

class PipelineShaderCreator
{
public:
	PipelineShaderCreator();
	~PipelineShaderCreator();


	// �쐬�֐�(�e�b�Z�[�V�����V�F�[�_�[�͖��Ή�)
	void Create(vk::Device* pLogicalDevice, 
		std::string vertexShaderPath	= DefaultShaderDefine.VertexShaderPath,
		std::string flagmentShaderPath	= DefaultShaderDefine.FragmentShaderPath);

	
	std::vector<vk::PipelineShaderStageCreateInfo>* GetPShaderStages();


private:
	vk::Device*					m_pLogicalDevice;

	// �V�F�[�_�[���W���[��
	vk::ShaderModule			m_VertexShaderModule;
	vk::ShaderModule			m_FragmentShaderModule;



	// �쐬���
	vk::PipelineShaderStageCreateInfo m_VertexStageInfo;
	vk::PipelineShaderStageCreateInfo m_FragmentStageInfo;

	vk::ShaderModule CreateShaderModule(vk::Device logicalDevice, const std::vector<char>& code);
	vk::PipelineShaderStageCreateInfo CreateShaderStage(vk::ShaderModule module, vk::ShaderStageFlagBits type);

	void DestroyModule();



};

