#pragma once
#include <vulkan/vulkan.hpp>
#include "NonCopyable.h"

/*
1. �f�B�X�N���v�^�Z�b�g�Ƃ́H
�f�B�X�N���v�^�Z�b�g�́A�V�F�[�_�[�Ƀf�[�^��n�����߂� �u�ڑ��|�C���g�v �̂悤�Ȃ��̂ł��B
�V�F�[�_�[���e�N�X�`���A�o�b�t�@�i�f�[�^�̔z��j�A�C���[�W�Ȃǂ������Ƃ��A
���̃f�[�^���ǂ��ɂ���̂��������邽�߂Ɏg���܂��B
Vulkan�ł̓f�[�^�����ڃV�F�[�_�[�ɓn�����̂ł͂Ȃ��A
�f�B�X�N���v�^�Z�b�g��ʂ��Ă��̈ʒu���w�����܂��B

2. �Ȃ��f�B�X�N���v�^�Z�b�g���K�v�Ȃ̂��H
Vulkan�͔��ɒ჌�x���ȃO���t�B�b�N�XAPI�ŁA
�V�X�e����������GPU�������̊Ǘ����K�v�ł��B�f�B�X�N���v�^�Z�b�g���g�����ƂŁA
�v���O������ �ǂ̃f�[�^���V�F�[�_�[�ɓn���� ���ȒP�ɊǗ��E�ύX�ł���悤�ɂȂ�܂��B
*/

class VDescriptorSetLayoutBase : public NonCopyable
{
public:
	VDescriptorSetLayoutBase(vk::DescriptorType type, vk::ShaderStageFlags stageFlag);
	~VDescriptorSetLayoutBase();

	virtual std::shared_ptr<vk::DescriptorSetLayout>	GetDescriptorSetLayout();
	virtual vk::DescriptorType							GetDescriptorType();
	const vk::ShaderStageFlags							GetStageFlag();

protected:
	vk::Device*									m_pLogicalDevice;	// �쐬���j���p�̃f�o�C�X���
	const vk::DescriptorType					m_DescriptorType;
	const vk::ShaderStageFlags					m_StageFlags;
	std::shared_ptr<vk::DescriptorSetLayout>	m_DescriptorSetLayout;


};

