#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <set>
#include <algorithm>
#include <array>

#include "GameWindow.h"
#include "Utilities.h"
#include "VulkanValidation.h"
#include "VulkanInstance.h"
#include "Surface.h"
#include "Device.h"
#include "ImagesGenerator.h"
#include "SwapChainUtility.h"
#include "RenderPassUtility.h"
#include "GraphicsPipelineUtility.h"
#include "FramebufferGenerator.h"
#include "CommandUtility.h"
#include "SynchronizationGenerator.h"
#include "WriteImage.cpp"

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

	int init(GameWindow renderWindow);
	void draw();
	void cleanup();

private:
	GLFWwindow* m_pWindow;

	int currentFrame = 0;

	// 拡張機能
	InstanceExtensionManager	m_InstanceExtension;
	CDeviceExtensionManager		m_DeviceExtension;

	// オブジェクト
	InstanceGenerator			m_InstanceGenerator;
	SurfaceGenerator			m_SurfaceGenerator;
	DeviceGenerator				m_DeviceGenerator;
	//ImagesGenerator			m_ImageGenerator;
	SwapchainGenerator			m_SwapchainGenerator;
	RenderpassGenerator			m_RenderpassGenerator;
	PipelineGenerator			m_PipelineGenerator;
	FramebufferGenerator		m_FramebufferGenerator;
	CommandGenerator			m_CommandGenerator;
	SynchronizationGenerator	m_SynchroGenerator;

	//コールバック
	VkDebugReportCallbackEXT callback;



	//デバイスの作成
	void createDebugCallback();
	void createLogicalDevice();


	//void createSurface();
	void createSwapChain();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSynchronisation();

	// - Record Functions
	void recordCommands();

	// - Get Functions
	void getPhysicalDevice();

	// - Support Functions
	// -- Checker Functions
	bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkValidationLayerSupport();
	bool checkDeviceSuitable(VkPhysicalDevice device) { return false; };
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