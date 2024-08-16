#include "Device.h"



DeviceGenerator::DeviceGenerator()
{
}

DeviceGenerator::~DeviceGenerator()
{
}

void DeviceGenerator::Create(CDeviceExtensionManager extensionManager, vk::Instance instance, vk::SurfaceKHR surface)
{
	m_bCreated = true;

	//使用可能な物理デバイスを探してくる
	PhysicalDeviceSelector physicalSelector(instance);
	auto selectorResult = physicalSelector.SelectSwapchainDevice(surface);
	m_PhysicalDevice = selectorResult.Handle;

	//論理デバイスの作成
	//デバイスの作成時にどんなキューを使用するか決める
	auto queueInfo = selectorResult.QueueInfo;
	auto logicalDeviceInfo = CreateDeviceInfo(extensionManager, m_PhysicalDevice, queueInfo);
	m_LogicalDevice = m_PhysicalDevice.createDevice(logicalDeviceInfo);
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

//vk::Queue DeviceGenerator::GetGraphicsQueue()
//{
//	CheckCreated();
//	return m_LogicalDevice.getQueue(m_QueueFamilyGenerator.GetGraphicIndex(), 0u);
//}

//vk::Queue DeviceGenerator::GetPresentationQueue()
//{
//	CheckCreated();
//	return m_LogicalDevice.getQueue(m_QueueFamilyGenerator.GetPresentationIndex(), 0u);
//}

//int DeviceGenerator::GetQueueIndex()
//{
//	return m_QueueFamilyGenerator.GetGraphicIndex();//ここではグラフィックスキューを渡しておく(後で修正)
//}

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

