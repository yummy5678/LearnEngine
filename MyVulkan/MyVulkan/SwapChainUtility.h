#pragma once
#include <vulkan/vulkan.hpp>
#include "QueueUtility.h"
#include "GraphicsDefine.h"
//#include "Utilities.h"

const int MAX_FRAME_DRAWS = 3;

struct SwapchainImage {
	vk::Image		image;		// Vulkan�̒��ň����摜
	vk::ImageView	imageView;	// �摜�r���[
};


struct SwapChainDetails {
	vk::SurfaceCapabilitiesKHR			surfaceCapabilities;	// Surface properties, e.g. image size/extent
	std::vector<vk::SurfaceFormatKHR>	formats;				// Surface image formats, e.g. RGBA and size of each colour
	std::vector<vk::PresentModeKHR>		presentationModes;		// How images should be presented to screen
};

namespace VulkanCreate
{
	// �X���b�v�`�F�C���̍쐬�֐�
	vk::SwapchainCreateInfoKHR GetSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
}

namespace VulkanUtility
{
	SwapChainDetails getSwapChainDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);


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


	std::vector<SwapchainImage> createSwapChainImages(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::SwapchainKHR swapchain);

	vk::ImageView createImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);

	std::vector<vk::Framebuffer> createFramebuffers(vk::Device logicalDevice, std::vector<SwapchainImage> swapChainImages, vk::RenderPass renderPass, vk::Extent2D extent);

};

