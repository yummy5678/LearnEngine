#include "VImage.h"

VImage::VImage() :
	m_Allocator(nullptr),
	m_ImageAllocation(nullptr),
	m_Image(),
	m_ImageCreateInfo(),
	m_ImageView()
{
}

VImage::~VImage()
{
	Cleanup();
}

void VImage::Create(VmaAllocator* allocator, vk::ImageCreateInfo createInfo, vk::ImageAspectFlags aspectFlag)
{
	// 初期化
	Cleanup();

	// イメージとアロケーションの作成
	CreateBuffer(allocator, createInfo);


	// VMAに紐づけられているオブジェクトの情報を取得
	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);
	// イメージビューの作成
	CreateImageView(allocatorInfo.device, m_Image, createInfo.format, aspectFlag);

}

void VImage::Cleanup()
{
	if (m_ImageAllocation) vmaDestroyImage(*m_Allocator, m_Image, m_ImageAllocation);
	m_Allocator = nullptr;
}

vk::Image VImage::GetImage()
{
	return m_Image;
}

vk::ImageView VImage::GetImageView()
{
	return m_ImageView;
}

void VImage::CreateBuffer(VmaAllocator* allocator, vk::ImageCreateInfo createInfo)
{
	VmaAllocationCreateInfo allocInfo;
	allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;  // 自動で最適なメモリを選択
	allocInfo.pool = VK_NULL_HANDLE;
	allocInfo.memoryTypeBits = 0;
	allocInfo.preferredFlags = 0;
	allocInfo.priority = 1;
	allocInfo.requiredFlags = 0;
	allocInfo.pUserData = nullptr;
	VkImage image = nullptr;
	auto imageInfo = (VkImageCreateInfo)createInfo;


	VkResult result = vmaCreateImage(*allocator, &imageInfo, &allocInfo, &image, &m_ImageAllocation, nullptr);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VMAによるイメージの作成に失敗しました!");
	}

	m_Image = vk::Image(image);  // VkImageをvk::Imageにキャスト
}

void VImage::CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag)
{
	vk::ImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.image = imageBuffer;
	imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
	imageViewCreateInfo.format = format;                                 // Image データのフォーマット(imageと同じものでなければならない)
	imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity; // RGBA コンポーネントを他の RGBA 値にリマップすることができます
	imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
	imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
	imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
	// Subresource は Image の一部だけを表示するための設定です
	//VK_IMAGE_ASPECT_COLOR_BIT
	imageViewCreateInfo.subresourceRange.aspectMask = aspectFlag;   // Image のどの面を表示するか (例: COLOR_BIT は色を表示するため)
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;          // 表示を開始する Mipmap レベル
	imageViewCreateInfo.subresourceRange.levelCount = 1;            // 表示する Mipmap レベルの数
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;        // 表示を開始する配列レベル
	imageViewCreateInfo.subresourceRange.layerCount = 1;            // 表示する配列レベルの数
	m_ImageView = logicalDevice.createImageView(imageViewCreateInfo);
}
