#pragma once
#include "VImageBufferBase.h"

class VImageBuffer :
    public VImageBufferBase
{
public:
    VImageBuffer(
		vk::ImageUsageFlags		bufferusage,
		vk::ImageAspectFlags	aspectFlag,
		vk::Format				format,
		vk::MemoryPropertyFlags	MemoryPrppertyFlag);

    VImageBuffer(
		vk::ImageUsageFlags bufferusage,
		vk::SharingMode	sharingMode,
		vk::ImageAspectFlags aspectFlag,
		vk::Format format,
		vk::MemoryPropertyFlags requiredMemoryPrppertyFlag,
		vk::MemoryPropertyFlags preferredMemoryPrppertyFlag,
		VmaAllocationCreateFlags allocationFlag);

    ~VImageBuffer();

    void Initialize(VmaAllocator* allocator, vk::Extent2D extent);
    void Cleanup();

private:

    




};

