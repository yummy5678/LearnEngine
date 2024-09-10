#pragma once
#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>


class DeviceExtensionManager
{
public:
	DeviceExtensionManager();	//コンストラクタ
	~DeviceExtensionManager();	//デストラクタ

	//拡張機能のリストを受け取る
	std::vector<const char*>* GetExtensions(vk::PhysicalDevice physicalDevice);

	//スワップチェーンの拡張機能を有効化
	void UseSwapchain();


private:
	bool m_bSwapchain = false;	//スワップチェーンの管理フラグ

	std::vector<const char*> m_ExtensionList;

	//スワップチェーンの拡張機能をリストに追加
	void CreateSwapChainExtension();

	// 引数の拡張機能の名前のものが利用できるか確認
	bool CheckExtensionsSupport(std::vector<const char*> checkExtensionNames, vk::PhysicalDevice physicalDevice);

};

