#pragma once
#include "VulkanInitializer.h"
#include "NonCopyable.h"
#include "RenderingPipeline.h"

class RendererBase : public NonCopyable
{
public:
	RendererBase(VulkanInitializer& initializer);
	~RendererBase();

	// 内部変数の初期化
	void Cleanup();

	virtual vk::Rect2D				GetSissorRect();
	virtual vk::Rect2D				GetRenderRect();
	virtual vk::Pipeline			GetPipeline()		= 0;
	virtual vk::PipelineLayout		GetPipelineLayout()	= 0;
	//virtual VShaderConfigureBase*	GetPShaderConfiguer()	= 0;



protected:
	vk::Device			m_LogicalDevice;
	vk::PhysicalDevice  m_PhygicalDevice;

	// 画面サイズ
	vk::Offset2D			m_Offset;	//描画範囲の始点
	vk::Extent2D			m_Extent;	//描画範囲の終点

	vk::Rect2D				m_RenderArea;	// フレーム内の描画位置の矩形

	// グラフィクスパイプラインの作成クラス
	RenderingPipeline m_GraphicsPipeline;





};

