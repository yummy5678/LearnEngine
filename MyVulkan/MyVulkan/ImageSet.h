#pragma once
#include <vulkan/vulkan.hpp>

// 画像のバッファとその設定を一緒に扱うための構造体
struct ImageSet
{
	vk::Image		buffer	= VK_NULL_HANDLE;
	vk::ImageView	view	= VK_NULL_HANDLE;
};

// 描画コマンド用にカラーイメージと深度イメージを一緒に取り扱うための構造体
struct RenderingImageSet
{
	ImageSet	color;
	ImageSet	depth;
};
