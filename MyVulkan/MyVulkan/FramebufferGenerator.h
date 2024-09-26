#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "VulkanUtility.h"
#include "SwapChainUtility.h"

class FramebufferGenerator : public CGeneratorBase
{
public:
	FramebufferGenerator();
	~FramebufferGenerator();

	void LoadShader(vk::Device logicalDevice, std::vector<vk::ImageView> imageViews, vk::RenderPass renderPass, vk::Extent2D extent);
	void LoadShader(vk::Device logicalDevice, std::vector<std::vector<vk::ImageView>> imageViews, vk::RenderPass renderPass, vk::Extent2D extent);



	std::vector<vk::Framebuffer>	GetFramebuffers();
private:
	vk::Device								m_LogicalDevice;
	std::vector<vk::Framebuffer>			m_Framebuffers;
	std::vector<vk::FramebufferCreateInfo>	m_FramebufferInfos;

	void Destroy();
	std::vector<std::vector<vk::ImageView>> CreateAttachments(std::vector<vk::ImageView> imageViews);
	std::vector<std::vector<vk::ImageView>> CreateAttachments(std::vector<std::vector<vk::ImageView>> imageViews);
	std::vector<vk::FramebufferCreateInfo> CreateFramebufferInfos(std::vector<std::vector<vk::ImageView>>& attachments, vk::RenderPass renderPass, vk::Extent2D extent);

};

// フレームバッファについて
// 描画結果出力先の情報を保持するオブジェクト。
// レンダーパスではアタッチメント(メモリ領域)にどのような処理を施すかを定義したが、
// フレームバッファではアタッチメント(メモリ領域)と
// レンダーパスで記述した処理(サブパス)の情報を結びつけ、
// 実際に描画結果を保持する役割を果たす。
//  
// renderPassについて
// どのレンダーパスと紐づけるつもりなのか設定する為の値。
//  
// アタッチメント(データ)について
// データの出力先。
// 最終的に表示する用の色データ、光の表現用の色データ、
// 深度情報、マスク情報等のように
// 複数のアタッチメントを設定することができる。
// 今回は最終表示用の色データだけあればいいので
// アタッチメントは一つだけ。
// 
// width・heightについて
// フレームバッファの幅と高さ。
// イメージの幅と高さと同じでなければならない。
// 
// layersについて
// 描画する視点の数(?)
// VRゲームで左右の視点を持つ場合などは「2」が入ったりする。
// 2Dや単純な3D表示なら「1」でいい。
// 
// 
// 
// 
// 