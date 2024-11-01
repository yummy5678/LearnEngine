#include "ViewportGenerator.h"


ViewportGenerator::ViewportGenerator()
{
	m_ClassName = "ViewportGenerator";
}

ViewportGenerator::~ViewportGenerator()
{
}

void ViewportGenerator::Create(vk::Extent2D extent)
{
	m_bCreated = true;
	m_ViewportStateCreateInfo = CreateInfo(extent);
}

vk::Viewport ViewportGenerator::GetViewport()
{
	CheckCreated();
	return m_Viewport;
}

vk::Rect2D ViewportGenerator::GetScissor()
{
	CheckCreated();
	return m_Scissor;
}

vk::PipelineViewportStateCreateInfo* ViewportGenerator::GetCreateInfoPointer()
{
	CheckCreated();
	return &m_ViewportStateCreateInfo;
}

vk::PipelineViewportStateCreateInfo ViewportGenerator::CreateInfo(vk::Extent2D extent)
{
	// -- �r���[�|�[�g & �V�U�[ --
	// �r���[�|�[�g���̍\���̂��쐬
	m_Viewport.x = 0.0f;									// x �J�n���W
	m_Viewport.y = 0.0f;									// y �J�n���W
	m_Viewport.width = (float)extent.width;				// �r���[�|�[�g�̕�
	m_Viewport.height = (float)extent.height;				// �r���[�|�[�g�̍���
	m_Viewport.minDepth = 0.0f;							// �t���[���o�b�t�@�̍ŏ��[�x
	m_Viewport.maxDepth = 1.0f;							// �t���[���o�b�t�@�̍ő�[�x

	// �V�U�[���̍\���̂��쐬
	m_Scissor.offset = VkOffset2D{ 0,0 };		// �g�p����̈�̃I�t�Z�b�g
	m_Scissor.extent = extent;				// �g�p����̈�͈̔͂ƃI�t�Z�b�g����J�n

	// �r���[�|�[�g�X�e�[�g�쐬����ݒ肷��
	vk::PipelineViewportStateCreateInfo viewportStateCreateInfo;
	viewportStateCreateInfo.sType = vk::StructureType::ePipelineViewportStateCreateInfo;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &m_Viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &m_Scissor;

	return viewportStateCreateInfo;
}
