#include "VImageBufferBase.h"

VImageBufferBase::VImageBufferBase(
	vk::ImageUsageFlags bufferusage,
	vk::SharingMode	sharingMode,
	vk::ImageAspectFlags aspectFlag,
	vk::Format format,
	vk::MemoryPropertyFlags requiredMemoryFlag,			// 使用するメモリの必須要件
	vk::MemoryPropertyFlags preferredMemoryFlag,		// 使用するメモリの優先要件
	VmaAllocationCreateFlags allocationFlag):
	m_pAllocator(nullptr),
	m_Usage(bufferusage),
	m_SharingMode(sharingMode),
	m_AspectFlag(aspectFlag),
	m_Format(format),
	m_ImageAllocation(VK_NULL_HANDLE),
	m_RequiredFlag(requiredMemoryFlag),
	m_PreferredFlag(preferredMemoryFlag),
	m_AllocationFlag(allocationFlag),
	m_Extent(0,0,0)
{
}

VImageBufferBase::~VImageBufferBase()
{
	Cleanup();
}

vk::Image VImageBufferBase::GetImageBuffer()
{
	return m_ImageSet.buffer;
}

vk::ImageView VImageBufferBase::GetImageView()
{
	return m_ImageSet.view;
}

ImageSet VImageBufferBase::GetImageSet()
{
	return m_ImageSet;
}

vk::Extent3D VImageBufferBase::GetExtent()
{
	return m_Extent;
}

vk::ImageUsageFlags VImageBufferBase::GetUsage()
{
	return m_Usage;
}

vk::SharingMode VImageBufferBase::GetSharingMode()
{
	return m_SharingMode;
}

vk::Format VImageBufferBase::GetFormat()
{
	return m_Format;
}

vk::ImageAspectFlags VImageBufferBase::GetAspectFlag()
{
	return m_AspectFlag;
}

void VImageBufferBase::Cleanup()
{
	if (m_pAllocator == nullptr) return;

	vmaDestroyImage(*m_pAllocator, m_ImageSet.buffer, m_ImageAllocation);
	m_ImageAllocation = VK_NULL_HANDLE;
	m_ImageSet = {};
	m_Extent = vk::Extent3D{};
	m_pAllocator = nullptr;
}



VmaAllocator* VImageBufferBase::GetUsingAllocator()
{
	return m_pAllocator;
}

VkImageCreateInfo VImageBufferBase::CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight)
{
	m_Extent = vk::Extent3D{ imageWidth, imageHeight, 1};

	vk::ImageCreateInfo imageCreateInfo;
	imageCreateInfo.imageType = vk::ImageType::e2D;			// 2Dイメージ
	imageCreateInfo.extent = m_Extent;	// 画像サイズ
	imageCreateInfo.mipLevels = 1;							// ミップマップのレベル
	imageCreateInfo.arrayLayers = 1;						// レイヤー数
	imageCreateInfo.format = m_Format;						// イメージフォーマット（RGBA8）
	imageCreateInfo.tiling = vk::ImageTiling::eOptimal;		// GPU最適なタイル形式
	imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;  // 初期レイアウト
	imageCreateInfo.usage = m_Usage;						// データの使用用途
	imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // マルチサンプリングの数
	imageCreateInfo.sharingMode = m_SharingMode;			// キュー間でのデータ共用の有無

	return imageCreateInfo;
}

VkImageCreateInfo VImageBufferBase::CreateImageInfo(vk::Extent3D extent)
{
	m_Extent = extent;

	vk::ImageCreateInfo imageCreateInfo;
	imageCreateInfo.imageType = vk::ImageType::e2D;			// 2Dイメージ
	imageCreateInfo.extent = m_Extent;						// イメージのサイズ
	imageCreateInfo.mipLevels = 1;							// ミップマップのレベル
	imageCreateInfo.arrayLayers = 1;						// レイヤー数
	imageCreateInfo.format = m_Format;						// イメージフォーマット（RGBA8）
	imageCreateInfo.tiling = vk::ImageTiling::eOptimal;		// GPU最適なタイル形式
	imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;  // 初期レイアウト
	imageCreateInfo.usage = m_Usage;						// データの使用用途
	imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // マルチサンプリングの数
	imageCreateInfo.sharingMode = m_SharingMode;			// キュー間でのデータ共用の有無

	return imageCreateInfo;
}

void VImageBufferBase::CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight)
{
	m_pAllocator = allocator;
	auto imageInfo = CreateImageInfo(imageWidth, imageHeight);

	// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
	VmaAllocationCreateInfo allocationInfo;
	allocationInfo.priority = 1.0f;
	allocationInfo.flags = m_AllocationFlag;
	allocationInfo.preferredFlags = (VkMemoryAllocateFlags)m_PreferredFlag;
	allocationInfo.requiredFlags = (VkMemoryAllocateFlags)m_RequiredFlag;
	allocationInfo.pool = VK_NULL_HANDLE;
	allocationInfo.memoryTypeBits = NULL;
	allocationInfo.pUserData = nullptr;
	allocationInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;

	VkImage image;
	VkResult result = vmaCreateImage(*allocator, &imageInfo, &allocationInfo, &image, &m_ImageAllocation, nullptr);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VMAによるイメージの作成に失敗しました!");
	}

	m_ImageSet.buffer = vk::Image(image);  // VkImageをvk::Imageにキャスト
}

void VImageBufferBase::CreateBuffer(VmaAllocator* allocator, vk::Extent2D extent)
{
	m_pAllocator = allocator;
	auto imageInfo = CreateImageInfo(vk::Extent3D{extent, 1});

	// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
	VmaAllocationCreateInfo allocationInfo;
	allocationInfo.priority = 1.0f;
	allocationInfo.flags = m_AllocationFlag;
	allocationInfo.preferredFlags = (VkMemoryAllocateFlags)m_PreferredFlag;
	allocationInfo.requiredFlags = (VkMemoryAllocateFlags)m_RequiredFlag;
	allocationInfo.pool = VK_NULL_HANDLE;
	allocationInfo.memoryTypeBits = NULL;
	allocationInfo.pUserData = nullptr;
	allocationInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;

	VkImage image;
	VkResult result = vmaCreateImage(*allocator, &imageInfo, &allocationInfo, &image, &m_ImageAllocation, nullptr);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VMAによるイメージの作成に失敗しました!");
	}

	m_ImageSet.buffer = vk::Image(image);  // VkImageをvk::Imageにキャスト
}

void VImageBufferBase::CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag)
{
	vk::ImageViewCreateInfo imageViewInfo;
	imageViewInfo.image = imageBuffer;
	imageViewInfo.viewType = vk::ImageViewType::e2D;
	imageViewInfo.format = format;
	imageViewInfo.components.r = vk::ComponentSwizzle::eIdentity;
	imageViewInfo.components.g = vk::ComponentSwizzle::eIdentity;
	imageViewInfo.components.b = vk::ComponentSwizzle::eIdentity;
	imageViewInfo.components.a = vk::ComponentSwizzle::eIdentity;
	imageViewInfo.subresourceRange.aspectMask = aspectFlag;
	imageViewInfo.subresourceRange.baseMipLevel = 0;
	imageViewInfo.subresourceRange.levelCount = 1;
	imageViewInfo.subresourceRange.baseArrayLayer = 0;
	imageViewInfo.subresourceRange.layerCount = 1;

	m_ImageSet.view = logicalDevice.createImageView(imageViewInfo);

}
