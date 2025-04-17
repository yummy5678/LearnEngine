#pragma once
//#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp> 
#include "NonCopyable.h"
#include "GraphicsDefine.h"
//#include "VulkanLayerManager.h"
#include "InstanceExtensionManager.h"
#include "DeviceExtensionManager.h"
#include "VulkanValidation.h"


// インスタンス拡張機能のリストを作成する
class InstanceGenerator : public NonCopyable
{
public:
	InstanceGenerator();
	~InstanceGenerator();

	void Create(InstanceExtension* extensionManager);
	void Destroy();

	vk::Instance GetInstanse();
	

private:
	vk::ApplicationInfo		m_ApplicationInfo;	//このアプリの名前などを入れる構造体
	vk::Instance			m_Instance;

	//コールバック
	// VkDebugReportCallbackEXT callback;	//非推奨
	vk::DebugUtilsMessengerEXT callback;

	// インスタンス拡張機能のリストを作成する
	std::vector<const char*> m_InstanceExtensions;

	void createDebugCallback();
	//アプリケーション情報の取得
	vk::ApplicationInfo				GetApplicationInfo();

	//インスタンスの作成情報の作成
	const vk::InstanceCreateInfo	GetInstanceInfo(
		const vk::ApplicationInfo* appInfo,
		InstanceExtension* extensionManager);

};

// インスタンスについて
// Vulkanの機能を利用するのに必要になるもの
// まず最初にインスタンスを作成する必要がある
// インスタンスは一つのアプリケーションに１つとは限らず、複数個作成できる。
// この後デバイス、GPUや周辺機器の情報を見て、必要な情報を作成する。
//

// アプリケーションインフォ(ApplicationInfo)について
// メッセージの表示用にアプリケーションの名前などを設定できる。
// アプリケーションインフォは作成しなくてもインスタンスを作成することはできる
// 

// レイヤーについて
// レイヤーをオンにしてエラーが出たときに情報が
// コマンドラインに表示出来るようになる。
// 詳しくはInstanceLayerManagerのページに記載。
// 

// 拡張機能について
// Vulkanを動かすにあたって必ずしも必要ではない機能は
// インスタンス作成時にオンにする。
// サーフェス、つまり画像の画面表示は拡張機能。
// 
