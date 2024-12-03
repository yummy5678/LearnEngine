#include "ImageBuffer.h"


VTextureBuffer::VTextureBuffer()
{
}

VTextureBuffer::~VTextureBuffer()
{
	Cleanup();
}

void VTextureBuffer::Cleanup()
{
	if(m_ImageAllocation) vmaDestroyImage(*m_Allocator, m_Buffer, m_ImageAllocation);
	m_Allocator = nullptr;
}

void VTextureBuffer::SetImage(VmaAllocator* allocator, Texture& texture)
{
	VStagingImageBuffer stagingBuffer;

	// �C���[�W�o�b�t�@���X�e�[�W���O�o�b�t�@��ʂ��č쐬
	CreateBuffer(allocator, texture.width, texture.height);
	stagingBuffer.Initialize(allocator, texture.width, texture.height, texture.channel);
	stagingBuffer.TransferDataToImageBuffer((void*)texture.data, m_Buffer);


	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);


	// �C���[�W�r���[�̍쐬
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

VkDescriptorSet VTextureBuffer::GetDescriptorSet()
{
	return VkDescriptorSet();
}

VkImageCreateInfo VTextureBuffer::CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight, vk::Format format, vk::ImageUsageFlags usage, vk::SharingMode mode)
{
	vk::ImageCreateInfo imageCreateInfo;
	imageCreateInfo.imageType = vk::ImageType::e2D;			// 2D�C���[�W
	imageCreateInfo.extent.width = imageWidth;				// �C���[�W�̕�
	imageCreateInfo.extent.height = imageHeight;			// �C���[�W�̍���
	imageCreateInfo.extent.depth = 1;						// 2D�C���[�W�Ȃ̂Ő[����1
	imageCreateInfo.mipLevels = 1;							// �~�b�v�}�b�v�̃��x��
	imageCreateInfo.arrayLayers = 1;						// ���C���[��
	imageCreateInfo.format = format;						// �C���[�W�t�H�[�}�b�g�iRGBA8�j
	imageCreateInfo.tiling = vk::ImageTiling::eOptimal;		// GPU�œK�ȃ^�C���`��
	imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;  // �������C�A�E�g
	imageCreateInfo.usage = usage;							// �f�[�^�̎g�p�p�r
	imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // �}���`�T���v�����O�̐�
	imageCreateInfo.sharingMode = mode;						// �L���[�Ԃł̃f�[�^���p�̗L��

	return imageCreateInfo;
}

void VTextureBuffer::CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight)
{
	auto imageInfo = CreateImageInfo(imageWidth, imageHeight, m_Format, m_Usage, m_SharingMode);


	VmaAllocationCreateInfo allocInfo;
	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;  // �����ōœK�ȃ�������I��
	VkImage image;
	VkResult result = vmaCreateImage(*allocator, &imageInfo, &allocInfo, &image, &m_ImageAllocation, nullptr);

	if (result != VK_SUCCESS) {
		throw std::runtime_error("VMA�ɂ��C���[�W�̍쐬�Ɏ��s���܂���!");
	}

	m_Buffer = vk::Image(image);  // VkImage��vk::Image�ɃL���X�g
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
		throw std::runtime_error("�C���[�W�r���[�̍쐬�Ɏ��s���܂���!");
	}

}

