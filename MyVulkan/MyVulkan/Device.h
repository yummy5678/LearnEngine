#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "GeneratorBase.h"
#include "QueueUtility.h"
#include "SwapChainUtility.h"

//使用する拡張機能
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class DeviceGenerator : CGeneratorBase
{
public:
	DeviceGenerator();
	~DeviceGenerator();

	void Create(vk::Instance instance, vk::SurfaceKHR surface);

	vk::PhysicalDevice	GetPhysicalDevice();
	vk::Device			GetLogicalDevice();
	

private:
	vk::PhysicalDevice	m_PhysicalDevice;
	vk::UniqueDevice	m_LogicalDevice;

	//論理デバイスの作成情報
	vk::DeviceCreateInfo m_DeviceInfo;

	//物理デバイスの取得
	vk::PhysicalDevice BringPhysicalDevice(vk::Instance instance, vk::SurfaceKHR surface);

	//論理デバイスの作成情報を作成
	void CreateDeviceInfo(std::vector< vk::DeviceQueueCreateInfo >* queueCreateInfos);
	//論理デバイスの作成
	void CreateLogicalDevice();



	//拡張機能が使えるかの確認
	bool CheckDeviceExtensionSupport(vk::PhysicalDevice device);
	bool CheckDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface);


};