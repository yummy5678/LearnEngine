#include "ImageBuffer.h"

VImageBuffer::VImageBuffer(
	vk::ImageUsageFlags bufferusage,
	vk::SharingMode	sharingMode,
	vk::ImageAspectFlags aspectFlag,
	vk::Format format,
	VkMemoryPropertyFlags requiredFlag) :
VImageBufferBase(
	bufferusage,
	vk::SharingMode::eExclusive
	vk::ImageAspectFlags
	vk::Format::
	requiredFlag,
	NULL,
	VMA_ALLOCATION_CREATE_STRATEGY_FIRST_FIT_BIT |
	VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT),
	m_Extent(0, 0)
{
}

VImageBuffer::VImageBuffer(
	vk::ImageUsageFlags bufferusage,
	vk::SharingMode	sharingMode,
	vk::ImageAspectFlags aspectFlag,
	vk::Format format,
	VkMemoryPropertyFlags requiredFlag,
	VkMemoryPropertyFlags preferredFlag,
	VmaAllocationCreateFlags allocationFlag) :
	VImageBufferBase(
		bufferusage,



		requiredFlag,
		preferredFlag,
		VMA_ALLOCATION_CREATE_STRATEGY_FIRST_FIT_BIT |
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT),
	m_Extent(0, 0)
{
}

VImageBuffer::~VImageBuffer()
{
}

void VImageBuffer::Initialize(VmaAllocator* allocator, vk::Extent2D extent)
{
	m_Extent = extent;

	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);

	// イメージの作成
	VImageBufferBase::CreateBuffer(allocator, extent);

	// イメージビューの作成
	VImageBufferBase::CreateImageView(allocatorInfo.device, m_ImageSet.buffer, m_Format, m_AspectFlag);
}
