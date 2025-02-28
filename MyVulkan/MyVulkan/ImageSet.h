#pragma once
#include <vulkan/vulkan.hpp>


struct ImageSet
{
	vk::Image		image;
	vk::ImageView	imageView;
};

// カラーイメージと深度イメージを一緒に取り扱うための構造体
// ただそれだけ。
struct RenderingImageSet
{
	ImageSet	color;
	ImageSet	depth;
};
