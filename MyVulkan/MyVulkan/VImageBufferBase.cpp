#include "VImageBufferBase.h"

VImageBufferBase::VImageBufferBase(vk::ImageUsageFlags bufferusage,
	VkMemoryPropertyFlags requiredFlag,			// 使用するバッファの必須要件
	VkMemoryPropertyFlags preferredFlag,		// 使用するバッファの優先要件
	VmaAllocationCreateFlags allocationFlag) :
	m_pAllocator(nullptr),
	m_AllocationFlag(),
	m_AspectFlag(vk::ImageAspectFlagBits::eColor),
	m_ImageBuffer(VK_NULL_HANDLE),
	m_Format(vk::Format::eR8G8B8A8Unorm),
	m_ImageAllocation(VK_NULL_HANDLE),
	m_ImageView(VK_NULL_HANDLE),
	m_RequiredFlag(requiredFlag),
	m_PreferredFlag(preferredFlag),
	m_SharingMode(vk::SharingMode::eExclusive),
	m_Usage(vk::ImageUsageFlagBits::eTransferDst)
{
	m_Usage = m_Usage | bufferusage;
}

VImageBufferBase::~VImageBufferBase()
{
}

vk::Image VImageBufferBase::GetImageBuffer()
{
	return m_ImageBuffer;
}

vk::ImageView VImageBufferBase::GetImageView()
{
	return m_ImageView;
}

VkImageCreateInfo VImageBufferBase::CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight)
{
	vk::ImageCreateInfo imageCreateInfo;
	imageCreateInfo.imageType = vk::ImageType::e2D;			// 2Dイメージ
	imageCreateInfo.extent.width = imageWidth;				// イメージの幅
	imageCreateInfo.extent.height = imageHeight;			// イメージの高さ
	imageCreateInfo.extent.depth = 1;						// 2Dイメージなので深さは1
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
	auto imageInfo = CreateImageInfo(imageWidth, imageHeight);

	// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
	VmaAllocationCreateInfo allocationInfo;
	allocationInfo.priority = 1.0f;
	allocationInfo.flags = m_AllocationFlag;
	allocationInfo.preferredFlags = m_PreferredFlag;
	allocationInfo.requiredFlags = m_RequiredFlag;
	allocationInfo.pool = VK_NULL_HANDLE;
	allocationInfo.memoryTypeBits = NULL;
	allocationInfo.pUserData = nullptr;
	allocationInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;	// 自動で最適なメモリを選択(通常はGPUローカルメモリ)

	VkImage image;
	VkResult result = vmaCreateImage(*allocator, &imageInfo, &allocationInfo, &image, &m_ImageAllocation, nullptr);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VMAによるイメージの作成に失敗しました!");
	}

	m_ImageBuffer = vk::Image(image);  // VkImageをvk::Imageにキャスト
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

	try
	{
		m_ImageView = logicalDevice.createImageView(imageViewInfo);
	}
	catch (const std::exception&)
	{
		throw std::runtime_error("イメージビューの作成に失敗しました!");
	}
}
