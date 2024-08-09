#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>


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
const std::unordered_map<InstanceLayerList, const char*> gLayerMap
{
	//{LAYER_VALIDATION,	"VK_LAYER_LUNARG_standard_validation"},
	{ LAYER_VALIDATION, "VK_LAYER_KHRONOS_validation" },

};


class InstanceLayerManager
{
public:
	InstanceLayerManager();
	~InstanceLayerManager();

	void AddLayer(InstanceLayerList layer);
	bool hasLayer(InstanceLayerList layer);
	bool DeleteLayer(InstanceLayerList layer);

	std::vector<const char*> GetLayerList();


private:

	std::vector<const char*> m_LayerList;





};

// Vulkanのレイヤーとは？
// 特定の指定したVulkanの処理に追加の動作を入れられる
// 主にパフォーマンスの確認やデバッグで使う
// バリデーション(検証)レイヤーはVulkanオブジェクトの生成等時に
// エラー情報を出してくれる。
//
// レイヤー名は"VK_LAYER_ 提供者名 _ 機能名"となっている
























