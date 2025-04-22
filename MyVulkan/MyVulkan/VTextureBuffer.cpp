#include "VTextureBuffer.h"


VMeshTextureBuffer::VMeshTextureBuffer() : 
	VImageBufferBase(
		vk::ImageUsageFlagBits::eSampled | 
		vk::ImageUsageFlagBits::eTransferDst,
		vk::SharingMode::eExclusive,
		vk::ImageAspectFlagBits::eColor,
		(vk::Format)VulkanDefine.UseColorFormat,
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

void VMeshTextureBuffer::Cleanup()
{
	printf("���b�V���摜��������܂�");
	VImageBufferBase::Cleanup();
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


	// �C���[�W�r���[�̍쐬
	VImageBufferBase::CreateImageView(allocatorInfo.device, m_ImageSet.buffer, m_Format, m_AspectFlag);

}
