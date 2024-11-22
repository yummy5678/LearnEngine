#pragma once

#include <vulkan/vulkan.hpp>
#include <stdexcept>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <VMA/vk_mem_alloc.h>
#include "DeviceExtensionManager.h"
#include "InstanceExtensionManager.h"
#include "Utilities.h"
#include "VulkanValidation.h"
#include "VulkanInstance.h"
#include "Device.h"

class VulkanInitializer
{
public:
	VulkanInitializer();
	~VulkanInitializer();

	int init();

	vk::Instance		GetInstance();
	vk::Device			GetLogicalDevice();
	vk::PhysicalDevice	GetPhysicalDevice();
	VmaAllocator*		GetPVmaAllocator();

	InstanceExtension*	GetPInstanceExtension();
	DeviceExtension*	GetPDeviceExtension();


	void cleanup();

	// �T�[�t�F�X���\���ɑΉ����Ă��邩�m�F
	bool CheckSupportSurface(VkSurfaceKHR surface);

private:
	// �g���@�\
	InstanceExtension	m_InstanceExtension;
	DeviceExtension		m_DeviceExtension;



	vk::Instance		m_Instance;
	vk::PhysicalDevice	m_PhysicalDevice;
	vk::Device			m_LogicalDevice;
	VmaAllocator		m_VmaAllocator;			//�A���P�[�^�[�F�����f�o�C�X�̃������m�ۂ�⏕���Ă����I�u�W�F�N�g

	//�R�[���o�b�N
	VkDebugReportCallbackEXT callback;


	//�f�o�C�X�̍쐬
	void createDebugCallback();
	//void createLogicalDevice();


	//
	//void createPushConstantRange();



	

	//void recordCommands(uint32_t currentImage);

	//// - Allocate Functions
	//void allocateDynamicBufferTransferSpace();

	//VkFormat chooseSupportedFormat(const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags featureFlags);

	//VkImage createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags,
	//	VkMemoryPropertyFlags propFlags, VkDeviceMemory* imageMemory);

	//int createTextureImage(std::string fileName);
	//int createTexture(std::string fileName);
	//int createTextureDescriptor(VkImageView textureImage);


	// -- Loader Functions
	//stbi_uc* loadTextureFile(std::string fileName, int* width, int* height, VkDeviceSize* imageSize);

	// �A���P�[�^�[�̍쐬
	void CreateAllocator(vk::Instance instance, vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);


};

// VulkanRenderer�N���X�ɂ���
// Vulkan�̃C���X�^���X�쐬����`��A�j���܂ł̏������܂Ƃ߂��N���X�B
// �ڍׂ͈ȉ��ɋL�ځB
// 
// �E��ʂɐ}�`��\������܂łɕK�v�Ȃ���
// 1. �C���X�^���X�̍쐬
// Vulkan���g�p����ۂ͉������܂��C���X�^���X���쐬����B
// �C���X�^���X��Vulkan�̋@�\���g�p����̂ɕK�v�Ȃ��́B
// 
// 2. �T�[�t�F�X�̍쐬
// �T�[�t�F�X(Vulkan�̃C���[�W�̕\����)�̏��𓾂�B
// �����GLFW(�E�B���h�E�쐬�p���C�u����)�ō쐬�����E�B���h�E������𓾂�B
// 
// 3. �����f�o�C�X�̑I��
// Vulkan�����p�ł��镨���f�o�C�X(GPU)��I������B
// (GPU�ɂ��Ă̏ڂ���������DeviceGenerator.h�ɋL��)
// �C���X�^���X����Đڑ�����Ă���GPU��S�Ď擾����
// �g�p�������@�\�ɑΉ��������̂������őI��ł����B
// 
// 4. �_���f�o�C�X�Ə�������(�L���[)�̍쐬
// GPU�ƒʐM���邽�߂ɁA�_���f�o�C�X���쐬����B
// �_���f�o�C�X�̓\�t�g�E�F�A����GPU�̏������������߂̂��́B
// ����͕`��ƕ\���������s�����߂̖���(�L���[)���擾����
// �_���f�o�C�X�ɓn���Ă����B
// 
// 5. �X���b�v�`�F�[���̐ݒ�
// �ȒP�ɐ�������ƁA�`�悵���摜���E�B���h�E�ɕ\������܂ł̓����ݒ肷����́B
// ���ꂪ�Ȃ��ƁA�`�挋�ʂ�\���ł��Ȃ��B
// ���e�������̂ŏڍׂ�SwapChainGenerator.h���Q�ƁB
// 
// 6. �����_�[�p�X�̐ݒ�
// �摜���쐬����ۂ̏����菇��ݒ肷�邽�߂̂��́B		 /////////////////////////////////////////��ŏC��
// �X���b�v�`�F�[���̍쐬����n���Ă���̂́A		   /  ////////////////////////////////////////��ŏC��
// �摜�f�[�^�̐F�̌`�����X���b�v�`�F�[���ƈ�v�����邽��/////////////////////////////////////////��ŏC��
// 
// 7. �O���t�B�b�N�X�p�C�v���C���̐ݒ�
// 3D�I�u�W�F�N�g�̃f�[�^��`�悷��ۂɎ{�������̐ݒ�B
// �����ŕ����̃V�F�[�_�[��ʂ��Ē��_���̏���
// �ŏI�I�ɉ摜�f�[�^�Ƃ��ĕϊ��ł���悤�ɂ���B
// �ڍׂ�PipelineGenerator.h���Q�ƁB
// 
// 8. �t���[���o�b�t�@�̍쐬
// �O���t�B�b�N�X�p�C�v���C�����`�悵���摜���Ǘ�������́B
// �ڍׂ�FramebufferGenerator.h���Q�ƁB
// 
// 9. �R�}���h�o�b�t�@�̍쐬�ƋL�^
// GPU�ɑ���p�̕`�施��(�R�}���h�o�b�t�@)��
// �X���b�v�`�F�[���ɕK�v�Ȗ������쐬����B
// ��CPU����GPU�ɏ������߂𓊂��鎞�̖��O���R�}���h�o�b�t�@�AGPU�����߂�ǂݎ��Ƃ��̌Ăѕ����L���[
// 
// 10. �����I�u�W�F�N�g�̍쐬
// �����I�u�W�F�N�g(�Z�}�t�H�ƃt�F���X)���쐬���āA�`��̐i�s�󋵂��Ǘ�����B
// �����I�u�W�F�N�g��GPU�̕`�揈���ƃf�B�X�v���C�̕\�����x�����킹�邽�߂̂��́B
// GPU�̌v�Z�͐��������̂ŁA���x�����킹�Ȃ��Ɖ�ʂւ̕\�����ǂ����Ȃ��Ȃ�B
// 
// 11. �`��̎��s
// 
// 
// 
// 
//  

//�����ł���Ă��邱��
//��ʂɎO�p�`��\������Ƃ���܂ł��
//�C���X�^���X��T�[�t�F�X�A�f�o�C�X�̍쐬�ɂ��Ă̏ڍׂ͊eGenerator�N���X�ɋL�ځB
// 
// 
// 
// 
// 
//