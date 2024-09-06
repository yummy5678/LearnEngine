#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "QueueUtility.h"
#include "GraphicsDefine.h"
#include "DeviceExtensionManager.h"
#include "ImagesGenerator.h"


class SwapchainGenerator : public CGeneratorBase
{
public:
	SwapchainGenerator(CDeviceExtensionManager& deviceExtensionManager);
	~SwapchainGenerator();

	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	void Destroy(vk::Device logicalDevice);

	vk::SwapchainKHR			GetSwapchain();
	vk::SwapchainCreateInfoKHR	GetSwapchainInfo();
	ImagesGenerator				GetImages();

private:
	vk::Device m_LogicalDevice;
	vk::SwapchainCreateInfoKHR			m_SwapchainInfo;
	vk::SwapchainKHR					m_Swapchain;

	ImagesGenerator						m_Images;

	// スワップチェーンの作成関数
	vk::SwapchainCreateInfoKHR CreateSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	//void CreateSwapChainImages(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainCreateInfoKHR m_SwapchainInfo);

	//vk::ImageView CreateImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
	// サーフェスの機能を取得
	//vk::SurfaceCapabilitiesKHR GetSurfaceCapabilities(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	// スワップチェーンのフォーマットを選択する関数
	vk::SurfaceFormatKHR SelectSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	// スワップチェーンのプレゼントモードを選択する関数
	vk::PresentModeKHR SelectPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

};

// スワップチェーンについて
// 画面に画像を表示するために必要なオブジェクト
// 表示用の画像はここで作成する
// サーフェスによって使用する値が決まっているものが多いので、
// よく確かめながら入力すること。

// 以下作成情報の項目について解説
// minImageCount
// スワップチェーンの中で扱う画像の数。
// サーフェスによって上限・下限枚数が決まっているのでその範囲内の数値を入れる。
// 作成した画像をそのまま表示するだけなら"1"
// ダブル・トリプルバッファリングにするなら"2"や"3"にする
// なるべく"3"にしたほうが滑らかになる

// imageFormat		:画像の形式(色のビット深度、チャネルの順序、圧縮形式、データの符号化方法等...)
// imageColorSpace	:画像の色空間
// 取り扱う画像の形式や用途。
// 内容はサーフェスによって決まっているので自分で決める必要はない
// 種類が多いので詳細については、以下のページを参照
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkFormat.html
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkColorSpaceKHR.html

// imageExtent
// 画像の縦横の大きさ
// 今回はサーフェスの値をそのまま設定する

// imageArrayLayers
// よく分からない項目。
// とりあえず平面の画像なら"1"にしとけばOK


// imageUsage
// 画像の使用用途の設定。
// サーフェスが対応している中から選んで決める
// スワップチェーンではVK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
// ()
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageUsageFlagBits.html

// imageSharingMode
// 画像の情報をキューの間でどのように扱うかを決める
// VK_SHARING_MODE_EXCLUSIVE	:単一のキューで画像データを扱う場合
// VK_SHARING_MODE_CONCURRENT	:複数のキューで画像データを扱う場合

// queueFamilyIndex・queueFamilyIndexCount
// よく分からない項目。
// とりあえず描画と表示用のキューが同じものなら
// 何も入れなくても良さそう。

// preTransform
// 画面に合わせて回転・反転等するかどうか決める
// SurfaceCapabilitiesの"supportedTransforms"から使用できるか確認できる

// compositeAlpha
// 画像のアルファ値(透明度)をどのように扱うか決める
// VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR			:透明度を扱わない。その分計算量が軽い。
// VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR	:画像の合成前にアルファ値を掛け合わせる
// VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR	:画像の合成後にアルファ値を掛け合わせる
// VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR			:プラットフォームで使用される合成方法を使用する(よく分からん)
// ※SurfaceCapabilitiesの"supportedCompositeAlpha"から使用できるか確認できる

// presentMode
// 描画した画像をどのように切り替えるかを決める
// モードによってはティアリング(描画速度と画面の更新速度の差によって画像がずれる現象)が
// 起こることもあるので目的に合わせて選ぶこと。
#pragma region プレゼントモードの種類
// VK_PRESENT_MODE_IMMEDIATE_KHR(即時モード):
// 垂直同期なしで画面更新が即座に行われる。
// ティアリングが起こる。
// 
// VK_PRESENT_MODE_MAILBOX_KHR(メールボックスモード):
// トリプルバッファリングが行われ、最新のイメージが常に表示される。
// ティアリングは起こさない。
// 
// VK_PRESENT_MODE_FIFO_KHR(FIFOモード):
// ダブルバッファリングに基づく垂直同期（VSync）方式。
// 画面の更新時に合わせて画像を表示するのでティアリングは起こらない。
// 
// VK_PRESENT_MODE_FIFO_RELAXED_KHR(FIFOリラックスモード):
// FIFOモードよりは遅延が少ないがティアリングは起こる。
// 
// 以下よく分らないモード
// VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR(需要リフレッシュ共有モード):
// デバイスの拡張機能"VK_KHR_shared_presentable_image"で使用できる。
// 需要に応じて画面更新を行うことで、省電力モードでの使用が想定されている。
// バッテリー駆動のデバイスや省電力を重視するモバイルデバイスで有効。
// ティアリングは少ない
// 
// VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR(連続リフレッシュ共有モード):
// デバイスの拡張機能"VK_KHR_shared_presentable_image"で使用できる。
// 連続的なリフレッシュ要求に基づいて画面更新が行われる。
// VR（仮想現実）やAR（拡張現実）アプリケーションなど、連続的な高速なリフレッシュが必要な場面で利用される。
// 低遅延でのリアルタイムな描画が求められる環境に適している。
// ティアリングは起こさない。
#pragma endregion

// clipped
// 画面の表示されていない部分まで計算するか決める
// "TRUE"で画面外は計算しなくなる
 
// oldSwapchain
// ウィンドウのサイズ変更や表示モードの変更等で
// スワップチェインを作り直す際に使用する。
// 








