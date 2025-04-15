#pragma once
#include <vulkan/vulkan.hpp>
#include "NonCopyable.h"
#include "ImageSet.h"

class RenderingTarget : public NonCopyable
{
public:
	RenderingTarget() = default;
	~RenderingTarget() = default;

	// 作成時に使用した論理デバイスを返す
	virtual vk::Device		GetLogicalDevice() = 0;	

	virtual vk::Extent2D	GetExtent()		= 0;
	virtual RenderingImageSet GetImageSet() = 0;
	virtual vk::Format		GetColorFormat()	= 0;
	virtual vk::Format		GetDepthFormat()	= 0;

};

