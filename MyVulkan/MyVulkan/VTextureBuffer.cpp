#include "VTextureBuffer.h"


VMeshTextureBuffer::VMeshTextureBuffer() : 
	VImageBufferBase(
		vk::ImageUsageFlagBits::eSampled | 
		vk::ImageUsageFlagBits::eTransferDst,
		vk::SharingMode::eExclusive,
		vk::ImageAspectFlagBits::eColor,
		vk::Format::eR8G8B8A8Unorm,
		vk::MemoryPropertyFlags(NULL),
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT | 
		VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT),
	m_Extent(0,0)
{
	
}

VMeshTextureBuffer::~VMeshTextureBuffer()
{
}

void VMeshTextureBuffer::SetImage(VmaAllocator* allocator, Texture& texture, vk::Fence fence)
{
	m_Extent.width = texture.width;
	m_Extent.height = texture.height;


	VImageBufferBase::CreateBuffer(allocator, texture.width, texture.height);


	VStagingImageBuffer stagingBuffer;
	stagingBuffer.Initialize(allocator, texture.width, texture.height, texture.channel);
	stagingBuffer.TransferHostDataToImageBuffer(texture.pixelData.data(), m_ImageSet.buffer, fence);


	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);


	// イメージビューの作成
	VImageBufferBase::CreateImageView(allocatorInfo.device, m_ImageSet.buffer, m_Format, m_AspectFlag);

}



//VkImageCreateInfo VMeshTextureBuffer::GetImageCreateInfo(uint32_t imageWidth, uint32_t imageHeight, vk::Format format, vk::ImageUsageFlags usage, vk::SharingMode mode)
//{
//	vk::ImageCreateInfo imageCreateInfo;
//	imageCreateInfo.imageType = vk::ImageType::e2D;			// 2Dイメージ
//	imageCreateInfo.extent.width = imageWidth;				// イメージの幅
//	imageCreateInfo.extent.height = imageHeight;			// イメージの高さ
//	imageCreateInfo.extent.depth = 1;						// 2Dイメージなので深さは1
//	imageCreateInfo.mipLevels = 1;							// ミップマップのレベル
//	imageCreateInfo.arrayLayers = 1;						// レイヤー数
//	imageCreateInfo.format = format;						// イメージフォーマット（RGBA8）
//	imageCreateInfo.tiling = vk::ImageTiling::eOptimal;		// GPU最適なタイル形式
//	imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;  // 初期レイアウト
//	imageCreateInfo.usage = usage;							// データの使用用途
//	imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // マルチサンプリングの数
//	imageCreateInfo.sharingMode = mode;						// キュー間でのデータ共用の有無
//
//	return imageCreateInfo;
//}
//
//void VMeshTextureBuffer::CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight)
//{
//	auto imageInfo = GetImageCreateInfo(imageWidth, imageHeight, m_Format, m_Usage, m_SharingMode);
//
//	// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
//	VmaAllocationCreateInfo dataAllocateInfo;
//	dataAllocateInfo.priority = 1.0f;
//	dataAllocateInfo.flags = m_AllocationFlag;
//	dataAllocateInfo.usage = m_MemoryUsage;	// 自動で最適なメモリを選択(通常はGPUローカルメモリ)
//	dataAllocateInfo.pool = VK_NULL_HANDLE;
//	dataAllocateInfo.memoryTypeBits = NULL;
//	dataAllocateInfo.preferredFlags = NULL;
//	dataAllocateInfo.requiredFlags = NULL;
//	dataAllocateInfo.pUserData = nullptr;
//
//
//	VmaAllocationCreateInfo allocInfo;
//	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;  // 自動で最適なメモリを選択
//	VkImage buffer;
//	VkResult result = vmaCreateImage(*allocator, &imageInfo, &allocInfo, &buffer, &m_ImageAllocation, nullptr);
//
//	if (result != VK_SUCCESS) 
//	{
//		throw std::runtime_error("VMAによるイメージの作成に失敗しました!");
//	}           
//
//	m_Buffer = vk::Image(buffer);  // VkImageをvk::Imageにキャスト
//}
//
//void VMeshTextureBuffer::CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag)
//{
//	vk::ImageViewCreateInfo imageViewInfo;
//	imageViewInfo.buffer = imageBuffer;
//	imageViewInfo.viewType = vk::ImageViewType::e2D;
//	imageViewInfo.format = format;
//	imageViewInfo.components.r = vk::ComponentSwizzle::eIdentity;
//	imageViewInfo.components.g = vk::ComponentSwizzle::eIdentity;
//	imageViewInfo.components.b = vk::ComponentSwizzle::eIdentity;
//	imageViewInfo.components.a = vk::ComponentSwizzle::eIdentity;
//	imageViewInfo.subresourceRange.aspectMask = aspectFlag;
//	imageViewInfo.subresourceRange.baseMipLevel = 0;
//	imageViewInfo.subresourceRange.levelCount = 1;
//	imageViewInfo.subresourceRange.baseArrayLayer = 0;
//	imageViewInfo.subresourceRange.layerCount = 1;
//
//	try
//	{
//		m_ImageView = logicalDevice.createImageView(imageViewInfo);
//	}
//	catch (const std::exception&)
//	{
//		throw std::runtime_error("イメージビューの作成に失敗しました!");
//	}
//
//}

