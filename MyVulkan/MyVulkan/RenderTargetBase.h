#pragma once
#include <vulkan/vulkan.hpp>
#include "ImageSet.h"

class RenderingTarget
{
public:
	RenderingTarget() = default;
	~RenderingTarget() = default;


	virtual vk::Extent2D	GetExtent()		= 0;
	virtual RenderingImageSet GetImageSet() = 0;
	virtual vk::Format		GetColorFormat()	= 0;
	virtual vk::Format		GetDepthFormat()	= 0;

};

