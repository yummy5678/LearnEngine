#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <vulkan/vulkan.hpp>

enum InstanceLayerList
{
	LAYER_VALIDATION,
	//Synchronization,
	//API_Dump,
	//Device_Simulation,
	//Screenshot
	LAYER_LIST_MAX_NUMBER
};

//インスタンスの番号と名前を結びつける
//constexpr const char VULKAN_LAYER_VALIDATION[] = "VK_LAYER_LUNARG_standard_validation";	// 旧バージョン
constexpr const char VULKAN_LAYER_VALIDATION[] = "VK_LAYER_KHRONOS_validation";



class InstanceLayerManager
{
public:
	InstanceLayerManager();
	~InstanceLayerManager();

	void Add(const char* layerName);
	bool hasLayer(const char* layerName);
	bool DeleteLayer(const char*);

	std::vector<const char*>* GetList();


private:
	
	std::vector<const char*> m_ExtensinList;	//内容の重複を避けたいためsetを使う

	//レイヤー名が使用できるか確認
	bool CheckLayerSupport(const char* validationLayers);
};

// Vulkanのレイヤーとは？
// 特定の指定したVulkanの処理に追加の動作を入れられる
// 主にパフォーマンスの確認やデバッグで使う
// バリデーション(検証)レイヤーはVulkanオブジェクトの生成等時に
// エラー情報を出してくれる。
//
// レイヤー名は"VK_LAYER_ 提供者名 _ 機能名"となっている
























