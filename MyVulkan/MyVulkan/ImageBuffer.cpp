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

void VImageBuffer::SetImage(VmaAllocator allocator, Texture& texture)
{
	VStagingImageBuffer stagingBuffer;

	CreateBuffer(allocator, texture.width, texture.height);

	stagingBuffer.Initialize(allocator, texture.width, texture.height, texture.channel);
	stagingBuffer.TransferDataToImageBuffer((void*)texture.data, m_Buffer);

}

vk::Image VImageBuffer::GetImageBuffer()
{
	return m_Buffer;
}

VkImageCreateInfo VImageBuffer::CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight, vk::Format format, vk::ImageUsageFlags usage, vk::SharingMode mode)
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

void VImageBuffer::CreateBuffer(VmaAllocator allocator, uint32_t imageWidth, uint32_t imageHeight)
{
	auto imageInfo = CreateImageInfo(imageWidth, imageHeight, m_Format, m_Usage, m_SharingMode);


	VmaAllocationCreateInfo allocInfo;
	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;  // �����ōœK�ȃ�������I��
	VkImage image;
	VkResult result = vmaCreateImage(allocator, &imageInfo, &allocInfo, &image, &m_ImageAllocation, nullptr);

	if (result != VK_SUCCESS) {
		throw std::runtime_error("VMA�ɂ��C���[�W�̍쐬�Ɏ��s���܂���!");
	}

	m_Buffer = vk::Image(image);  // VkImage��vk::Image�ɃL���X�g
}
