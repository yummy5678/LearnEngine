#pragma once
#include <vulkan/vulkan.hpp>
#include "ImageSet.h"

class RendererBase
{
public:
	RendererBase() = default;
	~RendererBase() = default;


	virtual vk::Extent2D	GetExtent()		= 0;
	virtual std::vector<ImageViewSet> GetImageSets() = 0;
	virtual vk::Format		GetColorFormat()	= 0;
	virtual vk::Format		GetDepthFormat()	= 0;

};

