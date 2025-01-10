#pragma once
#include <vulkan/vulkan.hpp>


// カラーイメージと深度イメージを一緒に取り扱うための構造体
// ただそれだけ。
struct ImageViewSet
{
	vk::ImageView color;
	vk::ImageView depth;
};
