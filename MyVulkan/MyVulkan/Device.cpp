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

	//将来コマンドは別の場所で作るようにする
	// コマンドバッファの作成
	m_CommandGenerator.Create(m_LogicalDevice, m_PhysicalDevice, 3);
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

vk::DeviceCreateInfo DeviceGenerator::CreateDeviceInfo(CDeviceExtensionManager& extensionManager, vk::PhysicalDevice physicalDevice, std::vector<vk::DeviceQueueCreateInfo>& queueCreateInfos)
{
	auto extension = extensionManager.GetExtensions(physicalDevice);

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

