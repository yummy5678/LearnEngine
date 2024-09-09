#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "QueueUtility.h"
#include "GraphicsDefine.h"
#include "DeviceExtensionManager.h"
#include "ImagesGenerator.h"


class SwapchainGenerator : public CGeneratorBase
{
public:
	SwapchainGenerator(CDeviceExtensionManager& deviceExtensionManager);
	~SwapchainGenerator();

	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	void Destroy(vk::Device logicalDevice);

	vk::SwapchainKHR			GetSwapchain();
	vk::SwapchainCreateInfoKHR	GetSwapchainInfo();
	ImagesGenerator				GetImages();

private:
	vk::Device m_LogicalDevice;
	vk::SwapchainCreateInfoKHR			m_SwapchainInfo;
	vk::SwapchainKHR					m_Swapchain;

	ImagesGenerator						m_Images;

	// �X���b�v�`�F�[���̍쐬�֐�
	vk::SwapchainCreateInfoKHR CreateSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	//void CreateSwapChainImages(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainCreateInfoKHR m_SwapchainInfo);

	//vk::ImageView CreateImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
	// �T�[�t�F�X�̋@�\���擾
	//vk::SurfaceCapabilitiesKHR GetSurfaceCapabilities(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	// �X���b�v�`�F�[���̃t�H�[�}�b�g��I������֐�
	vk::SurfaceFormatKHR SelectSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	// �X���b�v�`�F�[���̃v���[���g���[�h��I������֐�
	vk::PresentModeKHR SelectPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

};

// �X���b�v�`�F�[���ɂ���
// ��ʂɉ摜��\�����邽�߂ɕK�v�ȃI�u�W�F�N�g
// �\���p�̉摜�͂����ō쐬����
// �T�[�t�F�X�ɂ���Ďg�p����l�����܂��Ă�����̂������̂ŁA
// �悭�m���߂Ȃ�����͂��邱�ƁB

// �ȉ��쐬���̍��ڂɂ��ĉ��
// minImageCount
// �X���b�v�`�F�[���̒��ň����摜�̐��B
// �T�[�t�F�X�ɂ���ď���E�������������܂��Ă���̂ł��͈͓̔��̐��l������B
// �쐬�����摜�����̂܂ܕ\�����邾���Ȃ�"1"
// �_�u���E�g���v���o�b�t�@�����O�ɂ���Ȃ�"2"��"3"�ɂ���
// �Ȃ�ׂ�"3"�ɂ����ق������炩�ɂȂ�

// imageFormat		:�摜�̌`��(�F�̃r�b�g�[�x�A�`���l���̏����A���k�`���A�f�[�^�̕��������@��...)
// imageColorSpace	:�摜�̐F���
// ��舵���摜�̌`����p�r�B
// ���e�̓T�[�t�F�X�ɂ���Č��܂��Ă���̂Ŏ����Ō��߂�K�v�͂Ȃ�
// ��ނ������̂ŏڍׂɂ��ẮA�ȉ��̃y�[�W���Q��
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkFormat.html
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkColorSpaceKHR.html

// imageExtent
// �摜�̏c���̑傫��
// ����̓T�[�t�F�X�̒l�����̂܂ܐݒ肷��

// imageArrayLayers
// �悭������Ȃ����ځB
// �Ƃ肠�������ʂ̉摜�Ȃ�"1"�ɂ��Ƃ���OK


// imageUsage
// �摜�̎g�p�p�r�̐ݒ�B
// �T�[�t�F�X���Ή����Ă��钆����I��Ō��߂�
// �X���b�v�`�F�[���ł�VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
// ()
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageUsageFlagBits.html

// imageSharingMode
// �摜�̏����L���[�̊Ԃłǂ̂悤�Ɉ����������߂�
// VK_SHARING_MODE_EXCLUSIVE	:�P��̃L���[�ŉ摜�f�[�^�������ꍇ
// VK_SHARING_MODE_CONCURRENT	:�����̃L���[�ŉ摜�f�[�^�������ꍇ

// queueFamilyIndex�EqueueFamilyIndexCount
// �悭������Ȃ����ځB
// �Ƃ肠�����`��ƕ\���p�̃L���[���������̂Ȃ�
// ��������Ȃ��Ă��ǂ������B

// preTransform
// ��ʂɍ��킹�ĉ�]�E���]�����邩�ǂ������߂�
// SurfaceCapabilities��"supportedTransforms"����g�p�ł��邩�m�F�ł���

// compositeAlpha
// �摜�̃A���t�@�l(�����x)���ǂ̂悤�Ɉ��������߂�
// VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR			:�����x������Ȃ��B���̕��v�Z�ʂ��y���B
// VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR	:�摜�̍����O�ɃA���t�@�l���|�����킹��
// VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR	:�摜�̍�����ɃA���t�@�l���|�����킹��
// VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR			:�v���b�g�t�H�[���Ŏg�p����鍇�����@���g�p����(�悭�������)
// ��SurfaceCapabilities��"supportedCompositeAlpha"����g�p�ł��邩�m�F�ł���

// presentMode
// �`�悵���摜���ǂ̂悤�ɐ؂�ւ��邩�����߂�
// ���[�h�ɂ���Ă̓e�B�A�����O(�`�摬�x�Ɖ�ʂ̍X�V���x�̍��ɂ���ĉ摜������錻��)��
// �N���邱�Ƃ�����̂ŖړI�ɍ��킹�đI�Ԃ��ƁB
#pragma region �v���[���g���[�h�̎��
// VK_PRESENT_MODE_IMMEDIATE_KHR(�������[�h):
// ���������Ȃ��ŉ�ʍX�V�������ɍs����B
// �e�B�A�����O���N����B
// 
// VK_PRESENT_MODE_MAILBOX_KHR(���[���{�b�N�X���[�h):
// �g���v���o�b�t�@�����O���s���A�ŐV�̃C���[�W����ɕ\�������B
// �e�B�A�����O�͋N�����Ȃ��B
// 
// VK_PRESENT_MODE_FIFO_KHR(FIFO���[�h):
// �_�u���o�b�t�@�����O�Ɋ�Â����������iVSync�j�����B
// ��ʂ̍X�V���ɍ��킹�ĉ摜��\������̂Ńe�B�A�����O�͋N����Ȃ��B
// 
// VK_PRESENT_MODE_FIFO_RELAXED_KHR(FIFO�����b�N�X���[�h):
// FIFO���[�h���͒x�������Ȃ����e�B�A�����O�͋N����B
// 
// �ȉ��悭����Ȃ����[�h
// VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR(���v���t���b�V�����L���[�h):
// �f�o�C�X�̊g���@�\"VK_KHR_shared_presentable_image"�Ŏg�p�ł���B
// ���v�ɉ����ĉ�ʍX�V���s�����ƂŁA�ȓd�̓��[�h�ł̎g�p���z�肳��Ă���B
// �o�b�e���[�쓮�̃f�o�C�X��ȓd�͂��d�����郂�o�C���f�o�C�X�ŗL���B
// �e�B�A�����O�͏��Ȃ�
// 
// VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR(�A�����t���b�V�����L���[�h):
// �f�o�C�X�̊g���@�\"VK_KHR_shared_presentable_image"�Ŏg�p�ł���B
// �A���I�ȃ��t���b�V���v���Ɋ�Â��ĉ�ʍX�V���s����B
// VR�i���z�����j��AR�i�g�������j�A�v���P�[�V�����ȂǁA�A���I�ȍ����ȃ��t���b�V�����K�v�ȏ�ʂŗ��p�����B
// ��x���ł̃��A���^�C���ȕ`�悪���߂�����ɓK���Ă���B
// �e�B�A�����O�͋N�����Ȃ��B
#pragma endregion

// clipped
// ��ʂ̕\������Ă��Ȃ������܂Ōv�Z���邩���߂�
// "TRUE"�ŉ�ʊO�͌v�Z���Ȃ��Ȃ�
 
// oldSwapchain
// �E�B���h�E�̃T�C�Y�ύX��\�����[�h�̕ύX����
// �X���b�v�`�F�C������蒼���ۂɎg�p����B
// 








