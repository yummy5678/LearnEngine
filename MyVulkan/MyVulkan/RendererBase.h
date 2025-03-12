#pragma once
#include "VulkanInitializer.h"
#include "RenderingPipelineCreator.h"

class RendererBase
{
public:
	RendererBase(VulkanInitializer& initializer);
	~RendererBase();

	// �j��
	void Destroy();

	virtual vk::Rect2D				GetSissorRect();
	virtual vk::Rect2D				GetRenderRect();
	virtual vk::Pipeline			GetPipeline()		= 0;
	virtual vk::PipelineLayout		GetPipelineLayout()	= 0;
	//virtual VShaderConfigureBase*	GetPShaderConfiguer()	= 0;



protected:
	vk::Device*			m_pLogicalDevice;
	vk::PhysicalDevice* m_pPhygicalDevice;

	// ��ʃT�C�Y
	vk::Offset2D			m_Offset;	//�`��͈͂̎n�_
	vk::Extent2D			m_Extent;	//�`��͈͂̏I�_

	vk::Rect2D				m_RenderArea;	// �t���[�����̕`��ʒu�̋�`

	// �O���t�B�N�X�p�C�v���C���̍쐬�N���X
	RenderingPipelineCreator m_GraphicsPipeline;





};

