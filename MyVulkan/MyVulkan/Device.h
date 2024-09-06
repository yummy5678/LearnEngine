#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "GeneratorBase.h"
#include "PhysicalDeviceSelector.h"
#include "VulkanExtensions.h"
#include "QueueUtility.h"
#include "SwapChainUtility.h"
#include "CommandUtility.h"


class DeviceGenerator : CGeneratorBase
{
public:
	DeviceGenerator();
	~DeviceGenerator();

	void Create(CDeviceExtensionManager extensionManager,vk::Instance instance, vk::SurfaceKHR surface);
	void Destroy();

	vk::PhysicalDevice	GetPhysicalDevice();
	vk::Device			GetLogicalDevice();	

private:
	//物理デバイス
	vk::PhysicalDevice	m_PhysicalDevice;
	//論理デバイス
	vk::Device			m_LogicalDevice;
	//キュー
	std::vector<vk::DeviceQueueCreateInfo> m_QueueFamilyGenerator;
	//コマンドバッファの作成
	CommandGenerator	m_CommandGenerator;

	//論理デバイスの作成情報を作成
	vk::DeviceCreateInfo CreateDeviceInfo(CDeviceExtensionManager& extensionManager, vk::PhysicalDevice physicalDevice, std::vector<vk::DeviceQueueCreateInfo>& queueCreateInfos);
};


//以下デバイスに関するメモ
//VkPhysicalDeviceオブジェクトは明示的に破棄することはできませんVkInstance。
//代わりに、取得元のオブジェクトが破棄されると暗黙的に破棄されます。

