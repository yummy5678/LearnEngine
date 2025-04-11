#include "VImageBufferBase.h"

VImageBufferBase::VImageBufferBase(
	vk::ImageUsageFlags bufferusage,
	vk::SharingMode	sharingMode,
	vk::ImageAspectFlags aspectFlag,
	vk::Format format,
	vk::MemoryPropertyFlags requiredMemoryFlag,			// �g�p���郁�����̕K�{�v��
	vk::MemoryPropertyFlags preferredMemoryFlag,		// �g�p���郁�����̗D��v��
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
	imageCreateInfo.imageType = vk::ImageType::e2D;			// 2D�C���[�W
	imageCreateInfo.extent = m_Extent;	// �摜�T�C�Y
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
	m_Extent = extent;

	vk::ImageCreateInfo imageCreateInfo;
	imageCreateInfo.imageType = vk::ImageType::e2D;			// 2D�C���[�W
	imageCreateInfo.extent = m_Extent;						// �C���[�W�̃T�C�Y
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
