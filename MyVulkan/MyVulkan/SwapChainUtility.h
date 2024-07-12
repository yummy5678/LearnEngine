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

	// スワップチェインの作成関数
	vk::UniqueSwapchainKHR createSwapchain(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

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
};

