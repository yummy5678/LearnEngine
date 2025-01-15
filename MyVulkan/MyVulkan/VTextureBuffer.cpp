#include "VTextureBuffer.h"


VTextureBuffer::VTextureBuffer() : 
	m_Allocator			(VK_NULL_HANDLE),
	m_Buffer			(VK_NULL_HANDLE),
	m_ImageView			(VK_NULL_HANDLE),
	m_ImageAllocation	(VK_NULL_HANDLE),
	m_Usage				(vk::ImageUsageFlagBits::eTransferDst | // 転送先
						 vk::ImageUsageFlagBits::eSampled),		// サンプリングに使用
	m_SharingMode		(vk::SharingMode::eExclusive),
	m_Format			(vk::Format::eR8G8B8A8Unorm),
	m_AspectFlag		(vk::ImageAspectFlagBits::eColor),
	m_MemoryUsage		(VMA_MEMORY_USAGE_UNKNOWN),
	m_AllocationFlag	(VMA_ALLOCATION_CREATE_FLAG_BITS_MAX_ENUM)

{
	
}

VTextureBuffer::~VTextureBuffer()
{
}

void VTextureBuffer::SetImage(VmaAllocator* allocator, Texture& texture)
{
	VStagingImageBuffer stagingBuffer;

	// イメージバッファをステージングバッファを通して作成
	CreateBuffer(allocator, texture.width, texture.height);
	stagingBuffer.Initialize(allocator, texture.width, texture.height, texture.channel);
	stagingBuffer.TransferDataToImageBuffer((void*)texture.data, m_Buffer);


	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);


	// イメージビューの作成
	CreateImageView(allocatorInfo.device, m_Buffer, m_Format, m_AspectFlag);

}

vk::Image VTextureBuffer::GetImageBuffer()
{
	return m_Buffer;
}

vk::ImageView VTextureBuffer::GetImageView()
{
	return m_ImageView;
}


VkImageCreateInfo VTextureBuffer::CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight, vk::Format format, vk::ImageUsageFlags usage, vk::SharingMode mode)
{
	vk::ImageCreateInfo imageCreateInfo;
	imageCreateInfo.imageType = vk::ImageType::e2D;			// 2Dイメージ
	imageCreateInfo.extent.width = imageWidth;				// イメージの幅
	imageCreateInfo.extent.height = imageHeight;			// イメージの高さ
	imageCreateInfo.extent.depth = 1;						// 2Dイメージなので深さは1
	imageCreateInfo.mipLevels = 1;							// ミップマップのレベル
	imageCreateInfo.arrayLayers = 1;						// レイヤー数
	imageCreateInfo.format = format;						// イメージフォーマット（RGBA8）
	imageCreateInfo.tiling = vk::ImageTiling::eOptimal;		// GPU最適なタイル形式
	imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;  // 初期レイアウト
	imageCreateInfo.usage = usage;							// データの使用用途
	imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // マルチサンプリングの数
	imageCreateInfo.sharingMode = mode;						// キュー間でのデータ共用の有無

	return imageCreateInfo;
}

void VTextureBuffer::CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight)
{
	auto imageInfo = CreateImageInfo(imageWidth, imageHeight, m_Format, m_Usage, m_SharingMode);

	// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
	VmaAllocationCreateInfo dataAllocateInfo;
	dataAllocateInfo.priority = 1.0f;
	dataAllocateInfo.flags = m_AllocationFlag;
	dataAllocateInfo.usage = m_MemoryUsage;	// 自動で最適なメモリを選択(通常はGPUローカルメモリ)
	dataAllocateInfo.pool = VK_NULL_HANDLE;
	dataAllocateInfo.memoryTypeBits = NULL;
	dataAllocateInfo.preferredFlags = NULL;
	dataAllocateInfo.requiredFlags = NULL;
	dataAllocateInfo.pUserData = nullptr;


	VmaAllocationCreateInfo allocInfo;
	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;  // 自動で最適なメモリを選択
	VkImage image;
	VkResult result = vmaCreateImage(*allocator, &imageInfo, &allocInfo, &image, &m_ImageAllocation, nullptr);

	if (result != VK_SUCCESS) 
	{
		throw std::runtime_error("VMAによるイメージの作成に失敗しました!");
	}           

	m_Buffer = vk::Image(image);  // VkImageをvk::Imageにキャスト
}

void VTextureBuffer::CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag)
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

