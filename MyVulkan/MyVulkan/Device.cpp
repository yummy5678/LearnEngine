#include "Device.h"


DeviceGenerator::DeviceGenerator()
{
}

DeviceGenerator::~DeviceGenerator()
{
}

void DeviceGenerator::Create(DeviceExtension extensionManager, vk::Instance instance)
{
	m_bCreated = true;

	// 使用可能な物理デバイス(GPU)を探してくる
	PhysicalDeviceSelector physicalSelector(instance);

	// スワップチェインに対応した物理デバイスを探す
	//※ ここではサーフェスとの相性については見ない
	PhysicalDeviceContainer selectDevice = physicalSelector.SelectGraphicsDevice();
	m_PhysicalDevice = selectDevice.Handle;
	std::vector<vk::DeviceQueueCreateInfo> queueInfos = selectDevice.QueueInfo;


	auto extension = extensionManager.GetExtensions(m_PhysicalDevice);

	// 論理デバイスに設定する情報を作成
	vk::DeviceCreateInfo deviceInfo;
	deviceInfo.pNext;
	deviceInfo.flags;
	deviceInfo.queueCreateInfoCount = (uint32_t)queueInfos.size();	// キュー作成情報の数
	deviceInfo.pQueueCreateInfos = queueInfos.data();					// デバイスが必要とするキューを作成するためのキュー作成情報のリスト
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = nullptr;
	deviceInfo.enabledExtensionCount = (uint32_t)extension.size();			// 有効なロジカルデバイス拡張機能の数
	deviceInfo.ppEnabledExtensionNames = extension.data();					// 有効なロジカルデバイス拡張機能のリスト
	deviceInfo.pEnabledFeatures = nullptr;

	//論理デバイスの作成
	m_LogicalDevice = m_PhysicalDevice.createDevice(deviceInfo);


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

