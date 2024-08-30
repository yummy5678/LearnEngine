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

	void Create(vk::Device logicalDevice, std::vector<vk::ImageView> imageViews, vk::RenderPass renderPass, vk::Extent2D extent);



	std::vector<vk::Framebuffer>	GetFramebuffers();
private:
	vk::Device								m_LogicalDevice;
	std::vector<vk::Framebuffer>			m_Framebuffers;
	std::vector<vk::FramebufferCreateInfo>	m_FramebufferInfos;
	std::array<vk::ImageView, 1>			m_Attachments;

	void Destroy();
	std::vector<vk::FramebufferCreateInfo> CreateFramebufferInfos(std::vector<vk::ImageView> imageViews, vk::RenderPass renderPass, vk::Extent2D extent);

};

// フレームバッファについて
// 描画結果出力先の情報を保持するオブジェクト。
// レンダーパスではアタッチメント(メモリ領域)にどのような処理を施すかを定義したが、
// フレームバッファではアタッチメント(メモリ領域)と
// レンダーパスで記述した処理(サブパス)の情報を結びつけ、
// 実際に描画結果を保持する役割を果たす。
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
//  
// 