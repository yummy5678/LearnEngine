#include "VImageBufferBase.h"

VImageBufferBase::VImageBufferBase(vk::ImageUsageFlags bufferusage,
	VkMemoryPropertyFlags requiredFlag,			// �g�p����o�b�t�@�̕K�{�v��
	VkMemoryPropertyFlags preferredFlag,		// �g�p����o�b�t�@�̗D��v��
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

void VImageBufferBase::CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight)
{
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
	allocationInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;	// �����ōœK�ȃ�������I��(�ʏ��GPU���[�J��������)

	VkImage image;
	VkResult result = vmaCreateImage(*allocator, &imageInfo, &allocationInfo, &image, &m_ImageAllocation, nullptr);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VMA�ɂ��C���[�W�̍쐬�Ɏ��s���܂���!");
	}

	m_ImageBuffer = vk::Image(image);  // VkImage��vk::Image�ɃL���X�g
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
		throw std::runtime_error("�C���[�W�r���[�̍쐬�Ɏ��s���܂���!");
	}
}
