#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "GeneratorBase.h"
#include "VulkanExtensions.h"
#include "QueueUtility.h"
#include "SwapChainUtility.h"


class DeviceGenerator : CGeneratorBase
{
public:
	DeviceGenerator();
	~DeviceGenerator();

	void Create(CDeviceExtensionManager extensionManager,vk::Instance instance, vk::SurfaceKHR surface);
	void Destroy();

	vk::PhysicalDevice	GetPhysicalDevice();
	vk::Device			GetLogicalDevice();

	vk::Queue			GetGraphicsQueue();
	vk::Queue			GetPresentationQueue();
	int					GetQueueIndex();
	

private:
	vk::PhysicalDevice	m_PhysicalDevice;
	vk::Device			m_LogicalDevice;

	//論理デバイスの作成情報
	vk::DeviceCreateInfo m_DeviceInfo;

	//キュー
	QueueFamilyGenerator m_QueueFamilyGenerator;


	//物理デバイスの取得
	vk::PhysicalDevice pickPhysicalDevice(CDeviceExtensionManager extensionManager, vk::Instance instance, vk::SurfaceKHR surface);

	//論理デバイスの作成情報を作成
	vk::DeviceCreateInfo CreateDeviceInfo(CDeviceExtensionManager& extensionManager, vk::PhysicalDevice phygicalDevice, std::vector<vk::DeviceQueueCreateInfo>& queueCreateInfos);

	//拡張機能が使えるかの確認
	//bool CheckDeviceExtensionSupport(vk::PhysicalDevice physicalDevice);
	bool CheckDeviceSuitable(CDeviceExtensionManager extensionManager, vk::PhysicalDevice phygicalDevice, vk::SurfaceKHR surface);

	//QueueFamilyIndices getQueueFamilies(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);


	SwapChainDetails GetSwapChainDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
};


//以下デバイスに関するメモ
//VkPhysicalDeviceオブジェクトは明示的に破棄することはできませんVkInstance。
//代わりに、取得元のオブジェクトが破棄されると暗黙的に破棄されます。

