#pragma once
#include <vulkan/vulkan.hpp>
#include "Utilities.h"


class LogicalDevice
{
public:
	LogicalDevice();
	~LogicalDevice();

	void create();
	//void draw();

	VkDevice get();

private:
	vk::UniqueInstance instance;

	//デバイス
	VkPhysicalDevice physicalDevice;	//物理デバイス(ただの構造体なので破棄の必要はない)
	vk::UniqueDevice device;		//論理デバイス

	//キュー
	VkQueue graphicsQueue;
	VkQueue presentationQueue;

private:
	//デバイスの作成
	//void createDebugCallback();
	void createLogicalDevice();

	// - Get Functions
	void getPhysicalDevice();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);

};

