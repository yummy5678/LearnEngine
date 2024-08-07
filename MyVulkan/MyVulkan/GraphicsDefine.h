#pragma once
#include <vulkan/vulkan.h>

//�ŏ��ɍ쐬����E�B���h�E�̃T�C�Y���
constexpr uint32_t windowWidth  = 800;	//�E�B���h�E�̉���
constexpr uint32_t windowHeight = 600;	//�E�B���h�E�̏c��


// Vulkan�̐ݒ���
namespace VulkanDefine
{
	const bool LogMessageEnabled = true;	//Vulkan�̃I�u�W�F�N�g�쐬���Ƀ��O���o����
	const bool ValidationEnabled = true;	//���؃��C���[���g�p���邩


	const char*	ApplicationName = "Vulkan App";					// �A�v���P�[�V�����̖��O
	uint32_t	ApplicationVersion = VK_MAKE_VERSION(1, 0, 0);  // �A�v���P�[�V�����̃o�[�W������ 
	const char* EngineName = "No Engine";						// �G���W���̖��O
	uint32_t	EngineVersion = VK_MAKE_VERSION(1, 0, 0);		// �G���W���̃o�[�W������
	uint32_t	ApiVersion = VK_API_VERSION_1_0;                // Vulkan API�̃o�[�W����
}





