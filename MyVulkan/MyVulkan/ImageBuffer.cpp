#include "ImageBuffer.h"

VImageBuffer::VImageBuffer(
	vk::ImageUsageFlags bufferusage,
	vk::ImageAspectFlags aspectFlag,
	vk::Format format,
	vk::MemoryPropertyFlags requiredFlag) :
VImageBufferBase(
	bufferusage,
	vk::SharingMode::eExclusive,
	aspectFlag,
	format,
	requiredFlag,
	vk::MemoryPropertyFlags(NULL),
	VMA_ALLOCATION_CREATE_STRATEGY_FIRST_FIT_BIT |
	VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT)
{
}

VImageBuffer::VImageBuffer(
	vk::ImageUsageFlags bufferusage,
	vk::SharingMode	sharingMode,
	vk::ImageAspectFlags aspectFlag,
	vk::Format format,
	vk::MemoryPropertyFlags requiredFlag,
	vk::MemoryPropertyFlags preferredFlag,
	VmaAllocationCreateFlags allocationFlag) :
	VImageBufferBase(
		bufferusage,
		sharingMode,
		aspectFlag,
		format,
		requiredFlag,
		preferredFlag,
		VMA_ALLOCATION_CREATE_STRATEGY_FIRST_FIT_BIT |
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT)
{
}

VImageBuffer::~VImageBuffer()
{
}

void VImageBuffer::Initialize(VmaAllocator* allocator, vk::Extent2D extent)
{
	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);

	// イメージの作成
	VImageBufferBase::CreateBuffer(allocator, extent);

	// イメージビューの作成
	VImageBufferBase::CreateImageView(allocatorInfo.device, m_ImageSet.buffer, m_Format, m_AspectFlag);
}
