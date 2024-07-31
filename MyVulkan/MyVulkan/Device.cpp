#include "Device.h"


DeviceGenerator::DeviceGenerator()
{
	m_ClassName = "DeviceGenerator";
}

DeviceGenerator::~DeviceGenerator()
{
}

void DeviceGenerator::Create(vk::Instance instance, vk::SurfaceKHR surface)
{
	m_bCreated = true;

	//使用可能な物理デバイスを探してくる
	m_PhysicalDevice = BringPhysicalDevice(instance, surface);

	//論理デバイスの作成
	auto queueCreateInfos = VulkanCreate::GetQueueInfos(m_PhysicalDevice, surface);
	CreateDeviceInfo(&queueCreateInfos);
	m_LogicalDevice = m_PhysicalDevice.createDevice(m_DeviceInfo);
	
	// 選択した物理デバイスのキューファミリーインデックスを取得する
	QueueFamilyIndices queueIndex = getQueueFamilies(m_PhysicalDevice, surface);
	graphicsQueue = m_LogicalDevice.getQueue((uint32_t)queueIndex.graphicsFamily);
	//	 キューはデバイスと同時に作成されるため
	// キューへのハンドルを取得する
	//指定されたロジカルデバイスから、指定されたキューファミリーの指定されたキューインデックス（0は唯一のキューなので0）、指定されたVkQueueへの参照を置く
	vkGetDeviceQueue(logicalDevice.get(), queueIndex.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice.get(), queueIndex.presentationFamily, 0, &presentationQueue);
}

void DeviceGenerator::Destroy()
{
	vkDestroyDevice(m_LogicalDevice, nullptr);
}

vk::PhysicalDevice DeviceGenerator::GetPhysicalDevice()
{
	CheckCreated();
	return m_PhysicalDevice;
}

vk::Device DeviceGenerator::GetLogicalDevice()
{
	CheckCreated();
	return m_LogicalDevice;
}

vk::PhysicalDevice DeviceGenerator::BringPhysicalDevice(vk::Instance instance, vk::SurfaceKHR surface)
{
	//インスタンスから物理デバイス(GPU)を全て取得
	std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

	// 適切なデバイスが見つかるまでループする
	for (const auto& device : physicalDevices)
	{
		if (CheckDeviceSuitable(device, surface))
		{
			// 適切なデバイスが見つかった
			return device;
		}
	}

	// 利用可能なデバイスがない場合
	throw std::runtime_error("VulkanをサポートするGPUが見つかりません！");
}

void DeviceGenerator::CreateLogicalDevice()
{


}

void DeviceGenerator::CreateDeviceInfo(std::vector<vk::DeviceQueueCreateInfo>* queueCreateInfos)
{
	// 論理デバイスを作成するための情報を設定する
	m_DeviceInfo.pNext;
	m_DeviceInfo.flags;
	m_DeviceInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos->size();       // キュー作成情報の数
	m_DeviceInfo.pQueueCreateInfos = queueCreateInfos->data();                    // デバイスが必要とするキューを作成するためのキュー作成情報のリスト
	m_DeviceInfo.enabledLayerCount = 0;
	m_DeviceInfo.ppEnabledLayerNames = nullptr;
	m_DeviceInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();      // 有効なロジカルデバイス拡張機能の数
	m_DeviceInfo.ppEnabledExtensionNames = deviceExtensions.data();              // 有効なロジカルデバイス拡張機能のリスト
	m_DeviceInfo.pEnabledFeatures = nullptr;
}

bool DeviceGenerator::CheckDeviceExtensionSupport(vk::PhysicalDevice device)
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

bool DeviceGenerator::CheckDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface)
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
	QueueFamilyIndices indices = VulkanUtility::GetQueueFamilies(device, surface);

	// デバイスが必要とする拡張機能をサポートしているか確認する
	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	// Swapchainが有効かどうかを確認する
	bool swapChainValid = false;
	if (extensionsSupported)
	{
		// 特定の物理デバイスに対するSwap Chainの詳細を取得する
		SwapChainDetails swapChainDetails = VulkanUtility::getSwapChainDetails(device, surface);

		// Swap Chainの有効性を確認する。プレゼンテーションモードが空でなく、フォーマットも空でない場合に有効とみなす。
		swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();

	}

	// デバイスが適合しているかを確認する
	return indices.isValid() && extensionsSupported && swapChainValid;
}

QueueFamilyIndices DeviceGenerator::getQueueFamilies(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	QueueFamilyIndices indices;

	// 物理デバイスに対するすべてのキューファミリープロパティ情報を取得する
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyList.data());

	// 各キューファミリーを調べ、必要なタイプのキューを少なくとも1つ持っているかどうかを確認する
	int i = 0;
	for (const auto& queueFamily : queueFamilyList)
	{
		// まず、キューファミリーが少なくとも1つのキューを持っているか確認する（キューがない可能性もある）
		// キューはビットフィールドで複数のタイプを定義することができる。VK_QUEUE_*_BITとビットごとのAND演算を行い、必要なタイプを持っているか確認する
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;		// キューファミリーが有効であれば、そのインデックスを取得する
		}

		// キューファミリーがプレゼンテーションをサポートしているか確認する
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentationSupport);
		// キューがプレゼンテーションタイプであるかどうかを確認する（グラフィックスとプレゼンテーションの両方になり得る）
		if (queueFamily.queueCount > 0 && presentationSupport)
		{
			indices.presentationFamily = i;
		}

		// キューファミリーのインデックスが有効な状態にあるかどうかを確認し、そうであれば検索を停止する
		if (indices.isValid())
		{
			break;
		}

		i++;
	}

	return indices;
}
