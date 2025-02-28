#pragma once
#include <vulkan/vulkan.hpp>
#include "ImageSet.h"

class RenderTarget
{
public:
	RenderTarget() = default;
	~RenderTarget() = default;


	virtual vk::Extent2D	GetExtent()		= 0;
	virtual std::vector<RenderingImageSet> GetImageSets() = 0;
	virtual vk::Format		GetColorFormat()	= 0;
	virtual vk::Format		GetDepthFormat()	= 0;

};

