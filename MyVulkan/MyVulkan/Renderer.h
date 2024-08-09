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
#include "SwapChainUtility.h"
#include "RenderPassUtility.h"
#include "GraphicsPipelineUtility.h"
#include "FramebufferGenerator.h"
#include "CommandUtility.h"
#include "SynchronizationGenerator.h"


class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

	int init(GameWindow renderWindow);
	void draw();
	void cleanup();

private:
	GLFWwindow* window;

	int currentFrame = 0;
	InstanceGenerator		m_InstanceGenerator;
	SurfaceGenerator		m_SurfaceGenerator;
	DeviceGenerator			m_DeviceGenerator;
	SwapchainGenerator		m_SwapchainGenerator;
	RenderpassGenerator		m_RenderpassGenerator;
	PipelineGenerator		m_PipelineGenerator;
	FramebufferGenerator	m_FramebufferGenerator;
	CommandGenerator		m_CommandGenerator;


	
	// Vulkan Components
	//VkInstance instance;
	//vk::UniqueInstance instance;

	//コールバック
	VkDebugReportCallbackEXT callback;

	//メインデバイス
	//vk::PhysicalDevice physicalDevice;	//物理デバイス(ただの構造体なので破棄の必要はない)
	//vk::UniqueDevice logicalDevice;		//論理デバイス

	//キュー
	//VkQueue graphicsQueue;
	//VkQueue presentationQueue;

	//スワップチェイン
	//vk::UniqueSurfaceKHR				surface;
	//vk::UniqueSwapchainKHR			swapchain;
	//std::vector<SwapchainImage>			swapChainImages;
	//std::vector<vk::Framebuffer>		swapChainFramebuffers;
	//std::vector<vk::CommandBuffer>		commandBuffers;

	// - Pipeline
	//vk::UniquePipeline			graphicsPipeline;
	//vk::UniquePipelineLayout	pipelineLayout;
	//vk::UniqueRenderPass		renderPass;

	// - Pools
	//vk::UniqueCommandPool graphicsCommandPool;

	// - Utility
	//VkFormat swapChainImageFormat;
	//vk::Extent2D swapChainExtent = {
	//	windowWidth,
	//	windowHeight
	//};

	// - Synchronisation
	SynchronizationGenerator synchronizationGenerator;
	//std::vector<VkSemaphore> imageAvailable;
	//std::vector<VkSemaphore> renderFinished;
	//std::vector<VkFence>	 drawFences;

	// Vulkan Functions
	// - Create Functions
	//インスタンスの作成
	//void createInstance();
	// インスタンス拡張機能のリストを作成する
	std::vector<const char*> m_InstanceExtensions;

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

	// -- Getter Functions
	//QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
	SwapChainDetails getSwapChainDetails(VkPhysicalDevice device) { return SwapChainDetails{}; };

	// -- Choose Functions
	VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

	// -- Create Functions
	//VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	//VkShaderModule createShaderModule(const std::vector<char>& code);
};

// VulkanRendererクラスについて
// Vulkanのインスタンス作成から描画、破棄までの処理をまとめたクラス。
// 詳細は以下に記載。
// 
// ・画面に図形を表示するまでに必要なこと
// 1. インスタンスの作成
// Vulkanを使用する際は何よりもまずインスタンスを作成する。
// インスタンスはVulkanの機能を使用するのに必要なものである。
// 
// 2. サーフェスの作成
// サーフェス(Vulkanのイメージの表示先)の情報を得る。
// 今回の場合はGLFWを通して作成したウィンドウから情報を得る。
// 
// 3. 物理デバイスの選択
// Vulkanが利用できる物理デバイス(GPU)を選択する。
// (GPUについての詳しい説明はDeviceGenerator.hに記載)
// インスタンスを介して接続されているGPUを全て取得して
// 使用に適したものをここで選んでおく。
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
// 画像を作成する際の処理手順を設定するためのもの。
// スワップチェインの作成情報を渡しているのは、
// 画像データの色の形式をスワップチェインと一致させるため。
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
// スワップチェインに必要な枚数分作成する。
// ※CPUからGPUに処理命令を投げる時の名前がコマンドバッファ、GPUが命令を読み取るときの呼び方がキュー
// 
// 10. 同期オブジェクトの作成
// 同期オブジェクト(今回はセマフォとフェンス)を作成して、描画の進行状況を管理する。
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