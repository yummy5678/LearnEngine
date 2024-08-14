#include "DeviceExtensionManager.h"

CDeviceExtensionManager::CDeviceExtensionManager()
{
}

CDeviceExtensionManager::~CDeviceExtensionManager()
{
}

std::vector<const char*>* CDeviceExtensionManager::GetExtensions(vk::PhysicalDevice physicalDevice)
{
	//リストを初期化
	m_ExtensionList.clear();

	//スワップチェインが有効な場合、拡張機能を追加
	if (m_bSwapchain = true) CreateSwapChainExtension();

	if (CheckExtensionsSupport(m_ExtensionList, physicalDevice) == false)
	{
		//エラー
		//リストの拡張機能の名前をサポートしていなかった
		return nullptr;
	}

	//作成したリストを返す
	return &m_ExtensionList;
}

//スワップチェインの拡張機能を有効化(フラグで管理している)
void CDeviceExtensionManager::UseSwapchain()
{
	m_bSwapchain = true;
}

void CDeviceExtensionManager::CreateSwapChainExtension()
{
	m_ExtensionList.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

bool CDeviceExtensionManager::CheckExtensionsSupport(std::vector<const char*> checkExtensionNames, vk::PhysicalDevice physicalDevice)
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
