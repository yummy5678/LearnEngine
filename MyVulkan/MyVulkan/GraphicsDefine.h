#pragma once
#include <vulkan/vulkan.h>

// 最初に作成するウィンドウのサイズ情報
constexpr uint32_t windowWidth  = 800;	//ウィンドウの横幅
constexpr uint32_t windowHeight = 600;	//ウィンドウの縦幅

// スワップチェーンで使用するイメージの数
constexpr int MAX_FRAME_DRAWS = 2;

// Vulkanの設定情報
constexpr struct _VulkanDefine
{
	// デバッグ用フラグ
	bool LogMessageEnabled = true;	//Vulkanのオブジェクト作成時にログを出すか
	bool ValidationEnabled = true;	//検証レイヤーを使用するか

	// インスタンス作成用フラグ
	const char* ApplicationName = "Vulkan App";					// アプリケーションの名前
	uint32_t	ApplicationVersion = VK_MAKE_VERSION(1, 0, 0);  // アプリケーションのバージョン名 
	const char* EngineName = "No Engine";						// エンジンの名前
	uint32_t	EngineVersion = VK_MAKE_VERSION(1, 0, 0);		// エンジンのバージョン名
	uint32_t	ApiVersion = VK_API_VERSION_1_0;                // Vulkan APIのバージョン
} VulkanDefine;





