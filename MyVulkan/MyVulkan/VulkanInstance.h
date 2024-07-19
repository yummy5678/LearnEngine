#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "VulkanValidation.h"

// インスタンス拡張機能のリストを作成する


namespace VulkanCreate
{
	vk::ApplicationInfo		GetApplicationInfo();
	vk::InstanceCreateInfo	GetInstanceInfo(vk::ApplicationInfo* appInfo = nullptr);
}

namespace VulkanUtility
{
	bool CheckValidationLayerSupport(const std::vector<const char*> validationLayers);
	bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions);

	// GLFW でサーフェスを作るのに必要なインスタンス拡張を取得
	std::vector<const char*>* GetRequiredInstanceExtensionsPointer();

};

