#pragma once

#include <vulkan/vulkan.hpp>
#include <stdexcept>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <VMA/vk_mem_alloc.h>
#include "DeviceExtensionManager.h"
#include "InstanceExtensionManager.h"
#include "Utilities.h"
#include "VulkanValidation.h"
#include "VulkanInstance.h"
#include "Device.h"

class VulkanInitializer
{
public:
	VulkanInitializer();
	~VulkanInitializer();

	int init();

	vk::Instance		GetInstance();
	vk::Device			GetLogicalDevice();
	vk::PhysicalDevice	GetPhysicalDevice();
	VmaAllocator*		GetPVmaAllocator();

	InstanceExtension*	GetPInstanceExtension();
	DeviceExtension*	GetPDeviceExtension();


	void cleanup();

	// サーフェスが表示に対応しているか確認
	bool CheckSupportSurface(VkSurfaceKHR surface);

private:
	// 拡張機能
	InstanceExtension	m_InstanceExtension;
	DeviceExtension		m_DeviceExtension;



	vk::Instance		m_Instance;
	vk::PhysicalDevice	m_PhysicalDevice;
	vk::Device			m_LogicalDevice;
	VmaAllocator		m_VmaAllocator;			//アロケーター：物理デバイスのメモリ確保を補助してくれるオブジェクト

	//コールバック
	VkDebugReportCallbackEXT callback;


	//デバイスの作成
	void createDebugCallback();
	//void createLogicalDevice();


	//
	//void createPushConstantRange();



	

	//void recordCommands(uint32_t currentImage);

	//// - Allocate Functions
	//void allocateDynamicBufferTransferSpace();

	//VkFormat chooseSupportedFormat(const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags featureFlags);

	//VkImage createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags,
	//	VkMemoryPropertyFlags propFlags, VkDeviceMemory* imageMemory);

	//int createTextureImage(std::string fileName);
	//int createTexture(std::string fileName);
	//int createTextureDescriptor(VkImageView textureImage);


	// -- Loader Functions
	//stbi_uc* loadTextureFile(std::string fileName, int* width, int* height, VkDeviceSize* imageSize);

	// アロケーターの作成
	void CreateAllocator(vk::Instance instance, vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);


};

// VulkanRendererクラスについて
// Vulkanのインスタンス作成から描画、破棄までの処理をまとめたクラス。
// 詳細は以下に記載。
// 
// ・画面に図形を表示するまでに必要なこと
// 1. インスタンスの作成
// Vulkanを使用する際は何よりもまずインスタンスを作成する。
// インスタンスはVulkanの機能を使用するのに必要なもの。
// 
// 2. サーフェスの作成
// サーフェス(Vulkanのイメージの表示先)の情報を得る。
// 今回はGLFW(ウィンドウ作成用ライブラリ)で作成したウィンドウから情報を得る。
// 
// 3. 物理デバイスの選択
// Vulkanが利用できる物理デバイス(GPU)を選択する。
// (GPUについての詳しい説明はDeviceGenerator.hに記載)
// インスタンスを介して接続されているGPUを全て取得して
// 使用したい機能に対応したものをここで選んでおく。
// 
// 4. 論理デバイスと処理命令(キュー)の作成
// GPUと通信するために、論理デバイスを作成する。
// 論理デバイスはソフトウェア側でGPUの処理を扱うためのもの。
// 今回は描画と表示処理を行うための命令(キュー)を取得して
// 論理デバイスに渡しておく。
// 
// 5. スワップチェーンの設定
// 簡単に説明すると、描画した画像をウィンドウに表示するまでの動作を設定するもの。
// これがないと、描画結果を表示できない。
// 内容が多いので詳細はSwapChainGenerator.hを参照。
// 
// 6. レンダーパスの設定
// 画像を作成する際の処理手順を設定するためのもの。		 /////////////////////////////////////////後で修正
// スワップチェーンの作成情報を渡しているのは、		   /  ////////////////////////////////////////後で修正
// 画像データの色の形式をスワップチェーンと一致させるため/////////////////////////////////////////後で修正
// 
// 7. グラフィックスパイプラインの設定
// 3Dオブジェクトのデータを描画する際に施す処理の設定。
// ここで複数のシェーダーを通して頂点等の情報を
// 最終的に画像データとして変換できるようにする。
// 詳細はPipelineGenerator.hを参照。
// 
// 8. フレームバッファの作成
// グラフィックスパイプラインが描画した画像を管理するもの。
// 詳細はFramebufferGenerator.hを参照。
// 
// 9. コマンドバッファの作成と記録
// GPUに送る用の描画命令(コマンドバッファ)を
// スワップチェーンに必要な枚数分作成する。
// ※CPUからGPUに処理命令を投げる時の名前がコマンドバッファ、GPUが命令を読み取るときの呼び方がキュー
// 
// 10. 同期オブジェクトの作成
// 同期オブジェクト(セマフォとフェンス)を作成して、描画の進行状況を管理する。
// 同期オブジェクトはGPUの描画処理とディスプレイの表示速度を合わせるためのもの。
// GPUの計算は凄く速いので、速度を合わせないと画面への表示が追いつかなくなる。
// 
// 11. 描画の実行
// 
// 
// 
// 
//  

//ここでやっていること
//画面に三角形を表示するところまでやる
//インスタンスやサーフェス、デバイスの作成についての詳細は各Generatorクラスに記載。
// 
// 
// 
// 
// 
//