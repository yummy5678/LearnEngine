#pragma once
#include <vulkan/vulkan.hpp>
#include "QueueUtility.h"
#include "GraphicsDefine.h"
//#include "Utilities.h"

const int MAX_FRAME_DRAWS = 3;

struct SwapchainImage {
	vk::Image		image;		// Vulkanの中で扱う画像
	vk::ImageView	imageView;	// 画像ビュー
};


struct SwapChainDetails {
	vk::SurfaceCapabilitiesKHR			surfaceCapabilities;	// Surface properties, e.g. image size/extent
	std::vector<vk::SurfaceFormatKHR>	formats;				// Surface image formats, e.g. RGBA and size of each colour
	std::vector<vk::PresentModeKHR>		presentationModes;		// How images should be presented to screen
};

namespace VulkanCreate
{
	// スワップチェインの作成関数
	vk::SwapchainCreateInfoKHR GetSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
}

namespace VulkanUtility
{
	SwapChainDetails getSwapChainDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);


	// スワップチェインのフォーマットを選択する関数
	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

	// スワップチェインのプレゼントモードを選択する関数
	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

	// スワップチェインのエクステントを選択する関数
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

	// サーフェスの機能を取得
	vk::SurfaceCapabilitiesKHR getSurfaceCapabilities(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	// サーフェスフォーマットのリストを取得
	std::vector<vk::SurfaceFormatKHR> getSurfaceFormats(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	std::vector<vk::PresentModeKHR> getPresentModes(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);


	std::vector<SwapchainImage> createSwapChainImages(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::SwapchainKHR swapchain);

	vk::ImageView createImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);

	std::vector<vk::Framebuffer> createFramebuffers(vk::Device logicalDevice, std::vector<SwapchainImage> swapChainImages, vk::RenderPass renderPass, vk::Extent2D extent);

};

