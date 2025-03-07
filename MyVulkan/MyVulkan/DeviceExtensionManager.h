#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>


class DeviceExtension
{
public:
	DeviceExtension();	//コンストラクタ
	~DeviceExtension();	//デストラクタ

	vk::PhysicalDeviceVulkan13Features* GetCreateDevicePNext();
	vk::PhysicalDeviceFeatures*			GetUseDeviceFeatures();

	//拡張機能のリストを受け取る
	std::vector<const char*> GetEnabledExtensions(vk::PhysicalDevice physicalDevice);

	// 拡張機能を有効化
	void UseSwapchain();
	void UseMemoryBudget();
	void UseMemoryPriority();
	void UseDynamicRendering();
	void UseRenderingModeNonSolid();


private:
	std::vector<const char*> m_ExtensionList;
	vk::PhysicalDeviceFeatures m_DeviceFeatures;
	vk::PhysicalDeviceVulkan13Features m_DeviceExtensions;

	// 引数の拡張機能の名前のものが利用できるか確認
	bool CheckExtensionsSupport(std::vector<const char*> checkExtensionNames, vk::PhysicalDevice physicalDevice);
	// 入力した文字列があるか調べる関数
	bool CheckHasString(const char* target);
};

