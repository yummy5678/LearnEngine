#include "VImageBufferBase.h"

VImageBufferBase::VImageBufferBase(
	vk::ImageUsageFlags bufferusage,
	vk::SharingMode	sharingMode,
	vk::ImageAspectFlags aspectFlag,
	vk::Format format,
	VkMemoryPropertyFlags requiredFlag,			// �g�p���郁�����̕K�{�v��
	VkMemoryPropertyFlags preferredFlag,		// �g�p���郁�����̗D��v��
	VmaAllocationCreateFlags allocationFlag):
	m_pAllocator(nullptr),
	m_Usage(bufferusage),
	m_SharingMode(sharingMode),
	m_AspectFlag(aspectFlag),
	m_Format(format),
	m_ImageAllocation(VK_NULL_HANDLE),
	m_RequiredFlag(requiredFlag),
	m_PreferredFlag(preferredFlag),
	m_AllocationFlag(allocationFlag)
{
}

VImageBufferBase::~VImageBufferBase()
{
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

VkImageCreateInfo VImageBufferBase::CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight)
{
	vk::ImageCreateInfo imageCreateInfo;
	imageCreateInfo.imageType = vk::ImageType::e2D;			// 2D�C���[�W
	imageCreateInfo.extent.width = imageWidth;				// �C���[�W�̕�
	imageCreateInfo.extent.height = imageHeight;			// �C���[�W�̍���
	imageCreateInfo.extent.depth = 1;						// 2D�C���[�W�Ȃ̂Ő[����1
	imageCreateInfo.mipLevels = 1;							// �~�b�v�}�b�v�̃��x��
	imageCreateInfo.arrayLayers = 1;						// ���C���[��
	imageCreateInfo.format = m_Format;						// �C���[�W�t�H�[�}�b�g�iRGBA8�j
	imageCreateInfo.tiling = vk::ImageTiling::eOptimal;		// GPU�œK�ȃ^�C���`��
	imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;  // �������C�A�E�g
	imageCreateInfo.usage = m_Usage;						// �f�[�^�̎g�p�p�r
	imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // �}���`�T���v�����O�̐�
	imageCreateInfo.sharingMode = m_SharingMode;			// �L���[�Ԃł̃f�[�^���p�̗L��

	return imageCreateInfo;
}

VkImageCreateInfo VImageBufferBase::CreateImageInfo(vk::Extent3D extent)
{
	vk::ImageCreateInfo imageCreateInfo;
	imageCreateInfo.imageType = vk::ImageType::e2D;			// 2D�C���[�W
	imageCreateInfo.extent = extent;						// �C���[�W�̃T�C�Y
	imageCreateInfo.mipLevels = 1;							// �~�b�v�}�b�v�̃��x��
	imageCreateInfo.arrayLayers = 1;						// ���C���[��
	imageCreateInfo.format = m_Format;						// �C���[�W�t�H�[�}�b�g�iRGBA8�j
	imageCreateInfo.tiling = vk::ImageTiling::eOptimal;		// GPU�œK�ȃ^�C���`��
	imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;  // �������C�A�E�g
	imageCreateInfo.usage = m_Usage;						// �f�[�^�̎g�p�p�r
	imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // �}���`�T���v�����O�̐�
	imageCreateInfo.sharingMode = m_SharingMode;			// �L���[�Ԃł̃f�[�^���p�̗L��

	return imageCreateInfo;
}

void VImageBufferBase::CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight)
{
	m_pAllocator = allocator;
	auto imageInfo = CreateImageInfo(imageWidth, imageHeight);

	// CPU����GPU�֏��𑗂�̂ɓK�����������̈���쐬������
	VmaAllocationCreateInfo allocationInfo;
	allocationInfo.priority = 1.0f;
	allocationInfo.flags = m_AllocationFlag;
	allocationInfo.preferredFlags = m_PreferredFlag;
	allocationInfo.requiredFlags = m_RequiredFlag;
	allocationInfo.pool = VK_NULL_HANDLE;
	allocationInfo.memoryTypeBits = NULL;
	allocationInfo.pUserData = nullptr;
	allocationInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;

	VkImage image;
	VkResult result = vmaCreateImage(*allocator, &imageInfo, &allocationInfo, &image, &m_ImageAllocation, nullptr);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VMA�ɂ��C���[�W�̍쐬�Ɏ��s���܂���!");
	}

	m_ImageSet.buffer = vk::Image(image);  // VkImage��vk::Image�ɃL���X�g
}

void VImageBufferBase::CreateBuffer(VmaAllocator* allocator, vk::Extent2D extent)
{
	m_pAllocator = allocator;
	auto imageInfo = CreateImageInfo(vk::Extent3D{extent, 1});

	// CPU����GPU�֏��𑗂�̂ɓK�����������̈���쐬������
	VmaAllocationCreateInfo allocationInfo;
	allocationInfo.priority = 1.0f;
	allocationInfo.flags = m_AllocationFlag;
	allocationInfo.preferredFlags = m_PreferredFlag;
	allocationInfo.requiredFlags = m_RequiredFlag;
	allocationInfo.pool = VK_NULL_HANDLE;
	allocationInfo.memoryTypeBits = NULL;
	allocationInfo.pUserData = nullptr;
	allocationInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;

	VkImage image;
	VkResult result = vmaCreateImage(*allocator, &imageInfo, &allocationInfo, &image, &m_ImageAllocation, nullptr);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VMA�ɂ��C���[�W�̍쐬�Ɏ��s���܂���!");
	}

	m_ImageSet.buffer = vk::Image(image);  // VkImage��vk::Image�ɃL���X�g
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
