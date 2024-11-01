#include "ViewportGenerator.h"


ViewportGenerator::ViewportGenerator()
{
	m_ClassName = "ViewportGenerator";
}

ViewportGenerator::~ViewportGenerator()
{
}

void ViewportGenerator::Create(vk::Extent2D extent)
{
	m_bCreated = true;
	m_ViewportStateCreateInfo = CreateInfo(extent);
}

vk::Viewport ViewportGenerator::GetViewport()
{
	CheckCreated();
	return m_Viewport;
}

vk::Rect2D ViewportGenerator::GetScissor()
{
	CheckCreated();
	return m_Scissor;
}

vk::PipelineViewportStateCreateInfo* ViewportGenerator::GetCreateInfoPointer()
{
	CheckCreated();
	return &m_ViewportStateCreateInfo;
}

vk::PipelineViewportStateCreateInfo ViewportGenerator::CreateInfo(vk::Extent2D extent)
{
	// -- ビューポート & シザー --
	// ビューポート情報の構造体を作成
	m_Viewport.x = 0.0f;									// x 開始座標
	m_Viewport.y = 0.0f;									// y 開始座標
	m_Viewport.width = (float)extent.width;				// ビューポートの幅
	m_Viewport.height = (float)extent.height;				// ビューポートの高さ
	m_Viewport.minDepth = 0.0f;							// フレームバッファの最小深度
	m_Viewport.maxDepth = 1.0f;							// フレームバッファの最大深度

	// シザー情報の構造体を作成
	m_Scissor.offset = VkOffset2D{ 0,0 };		// 使用する領域のオフセット
	m_Scissor.extent = extent;				// 使用する領域の範囲とオフセットから開始

	// ビューポートステート作成情報を設定する
	vk::PipelineViewportStateCreateInfo viewportStateCreateInfo;
	viewportStateCreateInfo.sType = vk::StructureType::ePipelineViewportStateCreateInfo;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &m_Viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &m_Scissor;

	return viewportStateCreateInfo;
}
