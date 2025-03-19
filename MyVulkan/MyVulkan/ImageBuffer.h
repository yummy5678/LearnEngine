#pragma once
#include "VImageBufferBase.h"
class VImageBuffer :
    public VImageBufferBase
{
public:
    VImageBuffer(
		vk::ImageUsageFlags bufferusage,
		vk::SharingMode	sharingMode,
		vk::ImageAspectFlags aspectFlag,
		vk::Format format,
		VkMemoryPropertyFlags requiredFlag);

    VImageBuffer(
		vk::ImageUsageFlags bufferusage,
		vk::SharingMode	sharingMode,
		vk::ImageAspectFlags aspectFlag,
		vk::Format format,
		VkMemoryPropertyFlags requiredFlag,		
		VkMemoryPropertyFlags preferredFlag,		
		VmaAllocationCreateFlags allocationFlag);

    ~VImageBuffer();

    void Initialize(VmaAllocator* allocator, vk::Extent2D extent);
    void Cleanup();

private:

    vk::Extent2D m_Extent;




};

