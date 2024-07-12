#pragma once
#include <vulkan/vulkan.hpp>
#include "QueueUtility.h"
#include "GraphicsDefine.h"

struct SwapChainDetails {
	VkSurfaceCapabilitiesKHR surfaceCapabilities;		// Surface properties, e.g. image size/extent
	std::vector<VkSurfaceFormatKHR> formats;			// Surface image formats, e.g. RGBA and size of each colour
	std::vector<VkPresentModeKHR> presentationModes;	// How images should be presented to screen
};

namespace SwapChainUtility
{
	SwapChainDetails getSwapChainDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	// �X���b�v�`�F�C���̍쐬�֐�
	vk::UniqueSwapchainKHR createSwapchain(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	// �X���b�v�`�F�C���̃t�H�[�}�b�g��I������֐�
	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	// �X���b�v�`�F�C���̃v���[���g���[�h��I������֐�
	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
	// �X���b�v�`�F�C���̃G�N�X�e���g��I������֐�
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

	// �T�[�t�F�X�̋@�\���擾
	vk::SurfaceCapabilitiesKHR getSurfaceCapabilities(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	// �T�[�t�F�X�t�H�[�}�b�g�̃��X�g���擾
	std::vector<vk::SurfaceFormatKHR> getSurfaceFormats(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	std::vector<vk::PresentModeKHR> getPresentModes(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
};

