#pragma once
#include <vulkan/vulkan.h>

// 最初に作成するウィンドウのサイズ情報
constexpr uint32_t windowWidth  = 800;	//ウィンドウの横幅
constexpr uint32_t windowHeight = 600;	//ウィンドウの縦幅

// スワップチェーンで使用するイメージの数
constexpr int MAX_FRAME_DRAWS = 2;

constexpr uint64_t MAX_WAIT_TIME = 5'000'000'000;



// Vulkanの設定情報
constexpr struct _VulkanDefine
{
	// デバッグ用フラグ
	bool LogMessageEnabled = true;	//Vulkanのオブジェクト作成時にログを出すか
	bool ValidationEnabled = true;	//検証レイヤーを使用するか

	// インスタンス作成用フラグ
	const char* ApplicationName = "Graphics App";				// アプリケーションの名前
	uint32_t	ApplicationVersion = VK_MAKE_VERSION(1, 0, 0);  // アプリケーションのバージョン名 
	const char* EngineName = "Vulkan Engine";					// エンジンの名前
	uint32_t	EngineVersion = VK_MAKE_VERSION(1, 0, 0);		// エンジンのバージョン名
	uint32_t	ApiVersion = VK_API_VERSION_1_3;                // Vulkan APIのバージョン

	VkFormat UseColorFormat = VK_FORMAT_R8G8B8A8_UNORM;
	VkFormat UseDepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;

} VulkanDefine;

constexpr struct _ShaderDefine
{
	const char* VertexShaderPath	= "Shaders/vert.spv";
	const char* FragmentShaderPath	= "Shaders/frag.spv";

	const char* EntryName			= "main";

} DefaultShaderDefine;



