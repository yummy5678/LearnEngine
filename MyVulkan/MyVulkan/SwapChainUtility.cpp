#include "SwapChainUtility.h"

SwapChainDetails SwapChainUtility::getSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	SwapChainDetails swapChainDetails;

	// -- CAPABILITIES --
	// 特定の物理デバイスとサーフェスに対する表面のキャビティを取得する
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChainDetails.surfaceCapabilities);

	// -- FORMATS --
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	// フォーマットが返された場合、フォーマットのリストを取得する
	if (formatCount != 0)
	{
		swapChainDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChainDetails.formats.data());
	}

	// -- PRESENTATION MODES --
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, nullptr);

	// プレゼンテーションモードが返された場合、プレゼンテーションモードのリストを取得する
	if (presentationCount != 0)
	{
		swapChainDetails.presentationModes.resize(presentationCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, swapChainDetails.presentationModes.data());
	}

	return swapChainDetails;
}
