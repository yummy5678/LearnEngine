#include "Device.h"
#include "GraphicsDefine.h"


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

	auto pNext		= extensionManager.GetCreateDevicePNext();
	auto extension	= extensionManager.GetEnabledExtensions(m_PhysicalDevice);

	if (VulkanDefine.LogMessageEnabled == true)
	{
		std::printf("以下のデバイスエクステンションを使用します\n");
		for (size_t i = 0; i < extension.size(); i++)
		{
			std::cout << extension[i] << std::endl;
		}
	}

	// 論理デバイスに設定する情報を作成
	vk::DeviceCreateInfo deviceInfo;
	deviceInfo.pNext = pNext;
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

