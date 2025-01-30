#pragma once
#include "MeshObject.h"
#include "RenderObject.h"
#include "PipelineShaderCreator.h"
#include "RenderingPipelineCreator.h"
#include "VulkanInitializer.h"
#include "RendererBase.h"
#include "SceneCamera.h"
#include "VCameraDescriptor.h"
#include "VSingleDescriptor.h"

// �V�[���N���X����Vulkan�̃O���t�B�b�N�p�C�v���C���ւ�
// �`�惂�f���̏��̋��n�����s���N���X

// �V�F�[�_�[��`�掞�ׂ̍����ݒ��
// �����Ō��߂���悤�ɂ������B

// �܂����N���X
class RenderConfig
{
public:
	RenderConfig(VulkanInitializer& initializer);
	~RenderConfig();

	// ������
	//void Initialize(
	//	vk::Device logicalDevice,
	//	vk::Extent2D extent,
	//	vk::Format colorFomat,
	//	vk::Format depthFomat);
	void Initialize(RendererBase* renderere);

	// �f�B�X�N���v�^�Ȃǂ��X�V������
	void Update();

	// �j��
	void Destroy();

	vk::Rect2D			GetSissorRect();
	vk::Rect2D			GetRenderRect();
	vk::Pipeline		GetPipeline();
	vk::PipelineLayout	GetPipelineLayout();
	VModelShaderConfiguer* GetPShaderConfiguer();

	//std::vector<vk::DescriptorSet>			GetDescriptorSets();
	//std::vector<vk::DescriptorSetLayout>	GetDescriptorSetLayouts();
	//std::vector<vk::PushConstantRange>		GetPushConstantRanges();

	//void DrawImage(std::vector<RenderObject>* objects, SceneCamera* camera);

	void BindRenderingCommand(vk::CommandBuffer command, std::vector<RenderObject>* pObjects, SceneCamera* pCamera);

private:
	vk::Device* m_pLogicalDevice;
	vk::PhysicalDevice* m_pPhygicalDevice;

	// ��ʃT�C�Y
	vk::Offset2D			m_Offset;	//�`��͈͂̎n�_
	vk::Extent2D			m_Extent;	//�`��͈͂̏I�_

	vk::Rect2D				m_RenderArea;	// �t���[�����̕`��ʒu�̋�`

	RenderingPipelineCreator m_GraphicsPipeline;

	//�p�C�v���C���ɓn���V�F�[�_�[���̍쐬�N���X
	VModelShaderConfiguer	m_Shader;

	std::vector<std::weak_ptr<VMaterial*>>				m_RenderObjects;
	std::unordered_map<std::weak_ptr<VMaterial*>, VSingleDescriptor>	m_TextureDescriptors;

	//// �f�X�N���v�^
	//VTextureDescriptorLayout		m_TextureDescriptors;
	//VCameraDescriptor		m_CameraDescriptor;

	//// ���_���͏��	//ToDo:��Ő�p�̃N���X�ɕ�����
	//std::vector<vk::VertexInputBindingDescription>		m_BindingDescriptions;
	//std::vector<vk::VertexInputAttributeDescription>	m_AttributeDescriptions;
	//vk::PipelineVertexInputStateCreateInfo				m_InputStateInfo;


	//vk::PushConstantRange	GetPushConstantModelRange();
	//vk::PipelineVertexInputStateCreateInfo GetVertexInputState();
	void CreateTextureDescriptor(VMaterial* pMaterial);
	void UpdateTextureDescriptor();
};
