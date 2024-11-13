#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>


class DeviceExtensionCollector
{
public:
	DeviceExtensionCollector();	//コンストラクタ
	~DeviceExtensionCollector();	//デストラクタ

	//拡張機能のリストを受け取る
	std::set<const char*> GetExtensions(vk::PhysicalDevice physicalDevice);

	//スワップチェーンの拡張機能を有効化
	void UseSwapchain();
	void UseDynamicRendering();


private:
	std::set<const char*> m_ExtensionList;

	// 引数の拡張機能の名前のものが利用できるか確認
	bool CheckExtensionsSupport(std::vector<const char*> checkExtensionNames, vk::PhysicalDevice physicalDevice);

};

