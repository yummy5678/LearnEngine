#include "ImageBuffer.h"


VImageBuffer::VImageBuffer()
{
}

VImageBuffer::~VImageBuffer()
{
}

void VImageBuffer::Cleanup()
{
	if(m_ImageAllocation) vmaDestroyImage(m_Allocator, m_Buffer, m_ImageAllocation);
}

void VImageBuffer::SetImage(VmaAllocator allocator, Texture& texture, vk::Format format)
{
	VStagingImageBuffer stagingBuffer;

	vk::DeviceSize dataSize = texture.width * texture.height * texture.channel;
	CreateBuffer(allocator, texture.width, texture.height);

	stagingBuffer.Initialize(allocator, dataSize);
	stagingBuffer.TransferDataToImageBuffer((void*)texture.data, m_Buffer);



}

vk::Image VImageBuffer::GetImage()
{
	return m_Buffer;
}

VkImageCreateInfo VImageBuffer::CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight, vk::Format format, vk::ImageUsageFlags usage, vk::SharingMode mode)
{
	vk::ImageCreateInfo imageCreateInfo;
	imageCreateInfo.imageType = vk::ImageType::e2D;  // 2Dイメージ
	imageCreateInfo.extent.width = imageWidth;  // イメージの幅
	imageCreateInfo.extent.height = imageHeight;  // イメージの高さ
	imageCreateInfo.extent.depth = 1;  // 2Dイメージなので深さは1
	imageCreateInfo.mipLevels = 1;  // ミップマップのレベル
	imageCreateInfo.arrayLayers = 1;  // レイヤー数
	imageCreateInfo.format = format;  // イメージフォーマット（RGBA8）
	imageCreateInfo.tiling = vk::ImageTiling::eOptimal;  // GPU最適なタイル形式
	imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;  // 初期レイアウト
	imageCreateInfo.usage = usage;  // 転送先およびサンプリングに使用
	imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // マルチサンプリングの数
	imageCreateInfo.sharingMode = mode;  // 専有アクセス

	return imageCreateInfo;
}

void VImageBuffer::CreateBuffer(VmaAllocator allocator, uint32_t imageWidth, uint32_t imageHeight)
{
	auto imageInfo = CreateImageInfo(imageWidth, imageHeight, m_Format, m_Usage, m_SharingMode);


	VmaAllocationCreateInfo allocInfo;
	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;  // 自動で最適なメモリを選択
	VkImage image;
	VkResult result = vmaCreateImage(allocator, &imageInfo, &allocInfo, &image, &m_ImageAllocation, nullptr);

	if (result != VK_SUCCESS) {
		throw std::runtime_error("VMAによるイメージの作成に失敗しました!");
	}

	m_Buffer = vk::Image(image);  // VkImageをvk::Imageにキャスト
}
