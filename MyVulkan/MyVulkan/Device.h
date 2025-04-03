#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "NonCopyable.h"
#include "PhysicalDeviceSelector.h"
#include "DeviceExtensionManager.h"
#include "QueueUtility.h"


class DeviceGenerator : NonCopyable
{
public:
	DeviceGenerator();
	~DeviceGenerator();

	void Create(DeviceExtension extensionManager,vk::Instance instance);
	void Destroy();

	vk::PhysicalDevice	GetPhysicalDevice();
	vk::Device			GetLogicalDevice();	

private:
	//物理デバイス
	vk::PhysicalDevice	m_PhysicalDevice;
	//論理デバイス
	vk::Device			m_LogicalDevice;
	//キュー
	std::vector<vk::DeviceQueueCreateInfo> m_Queue;

};


//以下デバイスに関するメモ
//VkPhysicalDeviceオブジェクトは明示的に破棄することはできませんVkInstance。
//代わりに、取得元のオブジェクトが破棄されると暗黙的に破棄されます。

