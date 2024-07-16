#include "Device.h"

vk::PhysicalDevice DeviceUtility::getPhysicalDevice(vk::Instance& instance, vk::SurfaceKHR surface)
{

	vk::PhysicalDevice physicalDevice;	//物理デバイス

	//インスタンスから物理デバイスを取得
	std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

	// 適切なデバイスが見つかるまでループする
	for (const auto& device : physicalDevices)
	{
		if (checkDeviceSuitable(device, surface))
		{
			// 適切なデバイスが見つかった
			return device;
		}
	}

	// 利用可能なデバイスがない場合
	throw std::runtime_error("VulkanをサポートするGPUが見つかりません！");
	return vk::PhysicalDevice();
}


/// <summary>
/// デバイスが必要な拡張機能があるか確認する
/// </summary>
bool DeviceUtility::checkDeviceExtensionSupport(vk::PhysicalDevice device)
{
	// デバイスがサポートする拡張機能の数を取得する
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	// もし拡張機能が見つからない場合、失敗としてfalseを返す
	if (extensionCount == 0)
	{
		return false;
	}

	// 拡張機能の情報を保持するためのVkExtensionPropertiesのリストを作成する
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());

	// デバイスが必要とする拡張機能がサポートされているかチェックする
	for (const auto& deviceExtension : deviceExtensions)
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




bool DeviceUtility::checkDeviceSuitable(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	/*
	// デバイス自体に関する情報 (ID、名前、タイプ、ベンダーなど)
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	// デバイスの機能に関する情報 (ジオメトリシェーダー、テッセレーションシェーダー、ワイドラインなど)
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	*/

	// キューファミリーのインデックスを取得する
	QueueFamilyIndices indices = QueueUtility::getQueueFamilies(physicalDevice,surface);

	// デバイスが必要とする拡張機能をサポートしているか確認する
	bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

	// Swapchainが有効かどうかを確認する
	bool swapChainValid = false;
	if (extensionsSupported)
	{
		// 特定の物理デバイスに対するSwap Chainの詳細を取得する
		SwapChainDetails swapChainDetails = SwapChainUtility::getSwapChainDetails(physicalDevice, surface);

		// Swap Chainの有効性を確認する。プレゼンテーションモードが空でなく、フォーマットも空でない場合に有効とみなす。
		swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();

	}

	// デバイスが適合しているかを確認する
	return indices.isValid() && extensionsSupported && swapChainValid;
}


/// <summary>
/// 論理デバイスの作成
/// </summary>
vk::UniqueDevice DeviceUtility::createLogicalDevice(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	//論理デバイスの作成に必要なもの
	//1,使用するデバイスの拡張
	//2,使用するデバイスのレイヤー
	//3,デバイスのどのキューを使用するか

	//計算要求を受け付けるキューを探す
	auto queueCreateInfos = QueueUtility::getQueueInfos(physicalDevice, surface);

	// 論理デバイスを作成するための情報を設定する
	vk::DeviceCreateInfo deviceCreateInfo = {};
	//キューの設定
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());       // キュー作成情報の数
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();                                 // デバイスが必要とするキューを作成するためのキュー作成情報のリスト
	//拡張機能の設定
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());      // 有効なロジカルデバイス拡張機能の数
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();                           // 有効なロジカルデバイス拡張機能のリスト

	// 指定された物理デバイスに対してロジカルデバイスを作成する
	vk::UniqueDevice logicalDevice = physicalDevice.createDeviceUnique(deviceCreateInfo);
	if (!logicalDevice)
	{
		throw std::runtime_error("ロジカルデバイスの作成に失敗しました！");
	}

	return logicalDevice;
}

