#include "DeviceExtensionManager.h"

DeviceExtensionManager::DeviceExtensionManager()
{
}

DeviceExtensionManager::~DeviceExtensionManager()
{
}

std::set<const char*> DeviceExtensionManager::GetExtensions(vk::PhysicalDevice physicalDevice)
{
	//作成したリストを返す
	return m_ExtensionList;
}

//スワップチェーンの拡張機能を有効化(フラグで管理している)
void DeviceExtensionManager::UseSwapchain()
{
	m_ExtensionList.insert(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void DeviceExtensionManager::UseDynamicRendering()
{
	m_ExtensionList.insert(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
}

bool DeviceExtensionManager::CheckExtensionsSupport(std::vector<const char*> checkExtensionNames, vk::PhysicalDevice physicalDevice)
{
	// デバイスがサポートする拡張機能の数を取得する
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

	// もし拡張機能が見つからない場合、失敗としてfalseを返す
	if (extensionCount == 0)
	{
		return false;
	}

	// 拡張機能の情報を保持するためのVkExtensionPropertiesのリストを作成する
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());

	// デバイスが必要とする拡張機能がサポートされているかチェックする
	for (const auto& deviceExtension : checkExtensionNames)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			// 拡張機能名が一致するかどうかをstrcmpで比較する
			if (strcmp(deviceExtension, extension.extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}

		// 必要な拡張機能が見つからない場合はfalseを返す
		if (!hasExtension)
		{
			return false;
		}
	}

	// すべての必要な拡張機能が見つかった場合はtrueを返す
	return true;
}
