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
	// ������
	Cleanup();

	// �C���[�W�ƃA���P�[�V�����̍쐬
	CreateBuffer(allocator, createInfo);


	// VMA�ɕR�Â����Ă���I�u�W�F�N�g�̏����擾
	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);
	// �C���[�W�r���[�̍쐬
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
	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;  // �����ōœK�ȃ�������I��
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
		throw std::runtime_error("VMA�ɂ��C���[�W�̍쐬�Ɏ��s���܂���!");
	}

	m_Image = vk::Image(image);  // VkImage��vk::Image�ɃL���X�g
}

void VImage::CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag)
{
	vk::ImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.image = imageBuffer;
	imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
	imageViewCreateInfo.format = format;                                 // Image �f�[�^�̃t�H�[�}�b�g(image�Ɠ������̂łȂ���΂Ȃ�Ȃ�)
	imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity; // RGBA �R���|�[�l���g�𑼂� RGBA �l�Ƀ��}�b�v���邱�Ƃ��ł��܂�
	imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
	imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
	imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
	// Subresource �� Image �̈ꕔ������\�����邽�߂̐ݒ�ł�
	//VK_IMAGE_ASPECT_COLOR_BIT
	imageViewCreateInfo.subresourceRange.aspectMask = aspectFlag;   // Image �̂ǂ̖ʂ�\�����邩 (��: COLOR_BIT �͐F��\�����邽��)
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;          // �\�����J�n���� Mipmap ���x��
	imageViewCreateInfo.subresourceRange.levelCount = 1;            // �\������ Mipmap ���x���̐�
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;        // �\�����J�n����z�񃌃x��
	imageViewCreateInfo.subresourceRange.layerCount = 1;            // �\������z�񃌃x���̐�
	m_ImageView = logicalDevice.createImageView(imageViewCreateInfo);
}
