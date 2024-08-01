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

	queueFamilyGenerator.Create(m_PhysicalDevice, surface);

	//論理デバイスの作成
	//デバイスの作成時にどんなキューを使用するか決める
	m_DeviceInfo = CreateDeviceInfo(&queueFamilyGenerator.GetQueueInfos());
	m_LogicalDevice = m_PhysicalDevice.createDevice(m_DeviceInfo);

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

vk::Queue DeviceGenerator::GetGraphicsQueue()
{
	CheckCreated();
	return queueFamilyGenerator.GetGraphics(m_LogicalDevice);
}

vk::Queue DeviceGenerator::GetPresentationQueue()
{
	CheckCreated();
	return queueFamilyGenerator.GetPresentation(m_LogicalDevice);
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

vk::DeviceCreateInfo DeviceGenerator::CreateDeviceInfo(std::vector<vk::DeviceQueueCreateInfo>* m_QueueCreateInfos)
{
	// 論理デバイスを作成するための情報を設定する
	vk::DeviceCreateInfo deviceInfo;
	deviceInfo.pNext;
	deviceInfo.flags;
	deviceInfo.queueCreateInfoCount = (uint32_t)m_QueueCreateInfos->size();       // キュー作成情報の数
	deviceInfo.pQueueCreateInfos = m_QueueCreateInfos->data();                    // デバイスが必要とするキューを作成するためのキュー作成情報のリスト
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = nullptr;
	deviceInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();      // 有効なロジカルデバイス拡張機能の数
	deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();              // 有効なロジカルデバイス拡張機能のリスト
	deviceInfo.pEnabledFeatures = nullptr;

	return deviceInfo;
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


	// デバイスが必要とする拡張機能をサポートしているか確認する
	if(CheckDeviceExtensionSupport(device) == false) return false;	//拡張機能をサポートしていなかった！

	bool result = false;
	// 特定の物理デバイスに対するスワップチェインの詳細を取得する
	SwapChainDetails swapChainDetails = VulkanUtility::getSwapChainDetails(device, surface);
	// スワップチェインの有効性を確認する。プレゼンテーションモードが空でなく、フォーマットも空でない場合に有効とみなす。
	result = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();

	// 拡張機能に対応していてスワップチェインも有効
	return result;
}
