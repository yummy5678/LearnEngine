#include "Device.h"


DeviceGenerator::DeviceGenerator()
{
	m_ClassName = "DeviceGenerator";
}

DeviceGenerator::~DeviceGenerator()
{
}

void DeviceGenerator::Create(CDeviceExtensionManager extensionManager, vk::Instance instance, vk::SurfaceKHR surface)
{
	m_bCreated = true;

	//使用可能な物理デバイスを探してくる
	m_PhysicalDevice = BringPhysicalDevice(extensionManager,instance, surface);

	m_QueueFamilyGenerator.Create(m_PhysicalDevice, surface);

	//論理デバイスの作成
	//デバイスの作成時にどんなキューを使用するか決める
	auto queueInfos = m_QueueFamilyGenerator.GetQueueInfos();
	m_DeviceInfo = CreateDeviceInfo(extensionManager, m_PhysicalDevice, queueInfos);
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
	return m_LogicalDevice.getQueue(m_QueueFamilyGenerator.GetGraphicIndex(), 0u);
}

vk::Queue DeviceGenerator::GetPresentationQueue()
{
	CheckCreated();
	return m_LogicalDevice.getQueue(m_QueueFamilyGenerator.GetPresentationIndex(), 0u);
}

int DeviceGenerator::GetQueueIndex()
{
	return m_QueueFamilyGenerator.GetGraphicIndex();//ここではグラフィックスキューを渡しておく(後で修正)
}

vk::PhysicalDevice DeviceGenerator::BringPhysicalDevice(CDeviceExtensionManager extensionManager, vk::Instance instance, vk::SurfaceKHR surface)
{
	//インスタンスから物理デバイス(GPU)を全て取得
	std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

	// 適切なデバイスが見つかるまでループする
	for (const auto& device : physicalDevices)
	{
		if (CheckDeviceSuitable(extensionManager,device, surface))
		{
			// 適切なデバイスが見つかった
			return device;
		}
	}

	// 利用可能なデバイスがない場合
	throw std::runtime_error("VulkanをサポートするGPUが見つかりません！");
}

vk::DeviceCreateInfo DeviceGenerator::CreateDeviceInfo(CDeviceExtensionManager& extensionManager, vk::PhysicalDevice phygicalDevice, std::vector<vk::DeviceQueueCreateInfo>& queueCreateInfos)
{
	auto extension = extensionManager.GetExtensions(phygicalDevice);

	// 論理デバイスを作成するための情報を設定する
	vk::DeviceCreateInfo deviceInfo;
	deviceInfo.pNext;
	deviceInfo.flags;
	deviceInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();	// キュー作成情報の数
	deviceInfo.pQueueCreateInfos = queueCreateInfos.data();					// デバイスが必要とするキューを作成するためのキュー作成情報のリスト
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = nullptr;
	deviceInfo.enabledExtensionCount = (uint32_t)extension->size();			// 有効なロジカルデバイス拡張機能の数
	deviceInfo.ppEnabledExtensionNames = extension->data();					// 有効なロジカルデバイス拡張機能のリスト
	deviceInfo.pEnabledFeatures = nullptr;

	return deviceInfo;
}

//bool DeviceGenerator::CheckDeviceExtensionSupport(vk::PhysicalDevice physicalDevice)
//{
//	// デバイスがサポートする拡張機能の数を取得する
//	uint32_t extensionCount = 0;
//	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
//
//	// もし拡張機能が見つからない場合、失敗としてfalseを返す
//	if (extensionCount == 0)
//	{
//		return false;
//	}
//
//	// 拡張機能の情報を保持するためのVkExtensionPropertiesのリストを作成する
//	std::vector<VkExtensionProperties> extensions(extensionCount);
//	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());
//
//	// デバイスが必要とする拡張機能がサポートされているかチェックする
//	for (const auto& deviceExtension : deviceExtensions)
//	{
//		bool hasExtension = false;
//		for (const auto& extension : extensions)
//		{
//			// 拡張機能名が一致するかどうかをstrcmpで比較する
//			if (strcmp(deviceExtension, extension.extensionName) == 0)
//			{
//				hasExtension = true;
//				break;
//			}
//		}
//
//		// 必要な拡張機能が見つからない場合はfalseを返す
//		if (!hasExtension)
//		{
//			return false;
//		}
//	}
//
//	// すべての必要な拡張機能が見つかった場合はtrueを返す
//	return true;
//}

bool DeviceGenerator::CheckDeviceSuitable(CDeviceExtensionManager extensionManager, vk::PhysicalDevice device, vk::SurfaceKHR surface)
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
	if(extensionManager.GetExtensions(device) == nullptr) return false;	//拡張機能をサポートしていなかった！

	// 特定の物理デバイスに対するスワップチェインの詳細を取得する
	SwapChainDetails swapChainDetails = GetSwapChainDetails(device, surface);


	// プレゼンテーションモードが空でなく、フォーマットも空でない場合に有効とみなす。
	// 拡張機能に対応していてスワップチェインも有効
	return (!swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty());
}

SwapChainDetails DeviceGenerator::GetSwapChainDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	SwapChainDetails swapChainDetails;

	// -- CAPABILITIES --
	// 特定の物理デバイスとサーフェスに対する表面のキャパビリティを取得する
	swapChainDetails.surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);

	// -- FORMATS --
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	// フォーマットが返された場合、フォーマットのリストを取得する
	if (formatCount != 0)
	{
		swapChainDetails.formats.resize(formatCount);
		swapChainDetails.formats = physicalDevice.getSurfaceFormatsKHR(surface);
	}

	// -- PRESENTATION MODES --
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, nullptr);

	// プレゼンテーションモードが返された場合、プレゼンテーションモードのリストを取得する
	if (presentationCount != 0)
	{
		swapChainDetails.presentationModes.resize(presentationCount);
		swapChainDetails.presentationModes = physicalDevice.getSurfacePresentModesKHR(surface);
	}

	return swapChainDetails;
}
