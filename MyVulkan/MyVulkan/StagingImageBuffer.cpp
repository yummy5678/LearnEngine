#include "StagingImageBuffer.h"



VStagingImageBuffer::VStagingImageBuffer() :
	VBufferBase(
		vk::BufferUsageFlagBits::eTransferSrc | // ���̃o�b�t�@�̓R�s�[���Ƃ��ė��p����
		vk::BufferUsageFlagBits::eTransferDst , // ���̃o�b�t�@�̓������]����Ƃ��ė��p�ł���
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |	//CPU����A�N�Z�X�\
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT|
		VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
		NULL,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
		VMA_ALLOCATION_CREATE_MAPPED_BIT),
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
	m_CommandBuffer(VK_NULL_HANDLE),
	m_CommandPool(VK_NULL_HANDLE),
	m_ImageWidth(0),
	m_ImageHeight(0),
	m_ImageChannel(TEXTURE_CHANNEL_UNKNOWN),
	m_Queue(VK_NULL_HANDLE)

{
}

VStagingImageBuffer::~VStagingImageBuffer()
{
	Cleanup();
}

void VStagingImageBuffer::Initialize(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight, TextureChannel imageChannel)
{
	m_pAllocator = allocator;

	// �A���P�[�^�[�ɓo�^����Ă�������擾
	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*m_pAllocator, &allocatorInfo);

	m_LogicalDevice = vk::Device(allocatorInfo.device);
	m_PhysicalDevice = vk::PhysicalDevice(allocatorInfo.physicalDevice);
	m_ImageWidth = imageWidth;
	m_ImageHeight = imageHeight;
	m_ImageChannel = imageChannel;

	// �摜�̃f�[�^�T�C�Y
	vk::DeviceSize dataSize = imageWidth * imageHeight * imageChannel;

	// �]���p�L���[�̎擾
	QueueFamilySelector queueFamily;
	queueFamily.Initialize(m_PhysicalDevice);
	m_CommandPool = CreateCommandPool(m_LogicalDevice, queueFamily.GetTransferIndex());
	m_CommandBuffer = CreateCommandBuffer(m_LogicalDevice, m_CommandPool);
	m_Queue = m_LogicalDevice.getQueue(queueFamily.GetTransferIndex(), 0);

	auto stagingBufferInfo = CreateBufferInfo(dataSize, m_BufferUsage, m_SharingMode);


	// CPU����GPU�֏��𑗂�̂ɓK�����������̈���쐬������
	VmaAllocationCreateInfo stagingAllocateInfo;
	stagingAllocateInfo.priority = 1.0f;
	stagingAllocateInfo.flags = m_AllocationFlag;
	stagingAllocateInfo.requiredFlags = m_RequiredFlag;
	stagingAllocateInfo.preferredFlags = m_PreferredFlag;
	stagingAllocateInfo.pool = VK_NULL_HANDLE;
	stagingAllocateInfo.memoryTypeBits = NULL;
	stagingAllocateInfo.pUserData = nullptr;
	stagingAllocateInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;

	// �X�e�[�W���O�o�b�t�@�̍쐬
	vmaCreateBuffer(*m_pAllocator, &stagingBufferInfo, &stagingAllocateInfo, &m_Buffer, &m_Allocation, nullptr);


}

void VStagingImageBuffer::Cleanup()
{
	printf("�C���[�W�p�X�e�[�W���O�o�b�t�@��������܂�\n");
	VBufferBase::Cleanup();
}

void VStagingImageBuffer::TransferHostDataToImageBuffer(void* transferData, vk::Image toBuffer, vk::Fence fence)
{
	if (m_pAllocator == nullptr) throw std::runtime_error("��ɃX�e�[�W���O�o�b�t�@�̏��������s���Ă�������!\n");

	//VmaAllocationInfo allocInfo;
	//vmaGetAllocationInfo(*m_pAllocator, m_Allocation, &allocInfo);

	// �f�[�^��]���p�o�b�t�@�ɃR�s�[
	VBufferBase::MapData(m_AllocationInfo.pMappedData, transferData);

	// �]���p�o�b�t�@�̃f�[�^������̃C���[�W�o�b�t�@�ɃR�s�[
	SetCopyToImageCommand(m_CommandBuffer, m_Buffer, toBuffer, m_ImageWidth, m_ImageHeight);	// �]���R�}���h���쐬

	if(fence != VK_NULL_HANDLE)
	{
		m_LogicalDevice.waitForFences(fence, true, MAX_WAIT_TIME);
		m_LogicalDevice.resetFences(fence);
	}

	// �R�}���h�o�b�t�@�����s
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;				// �g���R�}���h��1�����ŏ[��
	submitInfo.pCommandBuffers = &m_CommandBuffer;	// �쐬�����R�}���h�o�b�t�@���Z�b�g

	vk::Result result = m_Queue.submit(1, &submitInfo, fence); // �R�}���h��GPU�̃L���[�ɑ��M
	if (result != vk::Result::eSuccess)
	{
		throw std::runtime_error("�摜�̃������Ԃ̈ړ��Ɏ��s���܂���\n");
	}

	if (fence == VK_NULL_HANDLE)
	{
		vkQueueWaitIdle(m_Queue);
	}
	else
	{
		m_LogicalDevice.waitForFences(fence, true, MAX_WAIT_TIME); // ������҂�
	}

}

void VStagingImageBuffer::TransferImageBufferToHostData(VImageBufferBase* transferBuffer, Texture* toData, vk::Fence fence)
{
	// NULL�`�F�b�N
	if (m_pAllocator == nullptr) throw std::runtime_error("��ɃX�e�[�W���O�o�b�t�@�̏��������s���Ă�������!");
	if(toData->pixelData.empty() == false) throw std::runtime_error("�]����͒��g�����Texture��p�ӂ��Ă�������!");

	// �t�F���X���ݒ肳��Ă���ꍇ�̓t�F���X�̏������I���܂ő҂�
	if (fence != VK_NULL_HANDLE)
	{
		m_LogicalDevice.waitForFences(fence, true, MAX_WAIT_TIME);
		m_LogicalDevice.resetFences(fence);
	}

	// �]�����̉摜�f�[�^��]���p�o�b�t�@�ɃR�s�[
	SetCopyImageToBufferCommand(					// �]���R�}���h���쐬
		m_CommandBuffer, 
		transferBuffer, 
		m_Buffer,
		m_ImageWidth, m_ImageHeight);
	vk::SubmitInfo submitInfo;						// �R�}���h�����s
	submitInfo.commandBufferCount = 1;				// �g���R�}���h��1�����ŏ[��
	submitInfo.pCommandBuffers = &m_CommandBuffer;	// �쐬�����R�}���h�o�b�t�@���Z�b�g
	m_Queue.submit(1, &submitInfo, fence);			// �R�}���h��GPU�̃L���[�ɑ��M
	
	if (fence == VK_NULL_HANDLE)
	{
		// �S�ẴL���[�̏�������������܂ő҂�
		vkQueueWaitIdle(m_Queue);
	}
	else
	{
		m_LogicalDevice.waitForFences(fence, true, MAX_WAIT_TIME); // �t�F���X�̊�����҂�
	}


	// �]���p�o�b�t�@�̃f�[�^������̃C���[�W�o�b�t�@�ɃR�s�[
	toData->channel = m_ImageChannel;
	toData->width = m_ImageWidth;
	toData->height = m_ImageHeight;
	//toData = m_LogicalDevice.mapMemory(toData, imgMem.get(), 0, imgMemReq.size);
	toData->pixelData.resize(m_ImageWidth * m_ImageHeight * m_ImageChannel);
	VBufferBase::MapData(toData->pixelData.data(), m_AllocationInfo.pMappedData);

}

vk::CommandPool VStagingImageBuffer::CreateCommandPool(vk::Device logicalDevice, uint32_t queueFamilyIndex)
{
	// �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.pNext = nullptr;
	poolInfo.queueFamilyIndex = queueFamilyIndex;
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eTransient;	// �R�}���h�o�b�t�@�̃��Z�b�g��������ꍇ�̓t���O��ǉ�����


	// �O���t�B�b�N�X�L���[�t�@�~���[�p�̃R�}���h�v�[�����쐬����
	try
	{
		return logicalDevice.createCommandPool(poolInfo);
	}
	catch (const std::exception&)
	{
		throw std::runtime_error("�]���p�R�}���h�v�[���̍쐬�Ɏ��s���܂����I");
	}
}

vk::CommandBuffer VStagingImageBuffer::CreateCommandBuffer(vk::Device logicalDevice, vk::CommandPool commandPool)
{
	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool = commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;	// �v���C�}���R�}���h�o�b�t�@
	allocInfo.commandBufferCount = 1;					// 1�̃R�}���h�o�b�t�@�����蓖��

	try
	{
		return logicalDevice.allocateCommandBuffers(allocInfo)[0];
	}
	catch (const std::exception&)
	{
		throw std::runtime_error("�]���p�R�}���h�o�b�t�@�̍쐬�Ɏ��s���܂����I");
	}

}

void VStagingImageBuffer::SetCopyToImageCommand(vk::CommandBuffer commandBuffer, vk::Buffer srcBuffer, vk::Image dstImage, uint32_t imageWidth, uint32_t imageHeight)
{
	// �R�}���h�o�b�t�@�̊J�n
	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
	commandBuffer.begin(beginInfo);

	// �C���[�W�̃��C�A�E�g��ύX(Undefined �� TransferDstOptimal)
	vk::ImageMemoryBarrier barrier;
	barrier.oldLayout = vk::ImageLayout::eUndefined;
	barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
	barrier.image = dstImage;
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = {};
	barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
		vk::PipelineStageFlagBits::eTransfer,
		{}, {}, {}, barrier);


	// �X�e�[�W���O�o�b�t�@����C���[�W�փR�s�[
	vk::BufferImageCopy copyRegion;
	copyRegion.bufferOffset = 0;
	copyRegion.bufferRowLength = 0;			// "0"���w�肵�Ă����Ǝ����I��Extent�̒l������
	copyRegion.bufferImageHeight = 0;		// "0"���w�肵�Ă����Ǝ����I��Extent�̒l������
	copyRegion.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
	copyRegion.imageSubresource.mipLevel = 0;
	copyRegion.imageSubresource.baseArrayLayer = 0;
	copyRegion.imageSubresource.layerCount = 1;
	copyRegion.imageOffset = vk::Offset3D{ 0, 0, 0 };
	copyRegion.imageExtent = vk::Extent3D{ imageWidth, imageHeight, 1 };

	commandBuffer.copyBufferToImage(srcBuffer, dstImage, vk::ImageLayout::eTransferDstOptimal, { copyRegion });

	// �C���[�W�̃��C�A�E�g��ύX�iTransferDstOptimal �� ShaderReadOnlyOptimal�j
	barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
	barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
		vk::PipelineStageFlagBits::eFragmentShader,
		{}, {}, {}, barrier);

	// �R�}���h�o�b�t�@�̏I��
	commandBuffer.end();
}

void VStagingImageBuffer::SetCopyImageToBufferCommand(
	vk::CommandBuffer commandBuffer, 
	VImageBufferBase* srcImage, 
	vk::Buffer dstBuffer, 
	uint32_t imageWidth, 
	uint32_t imageHeight)
{
	// �R�}���h�o�b�t�@�̊J�n
	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
	commandBuffer.begin(beginInfo);

	// �C���[�W�̃��C�A�E�g��TRANSFER_SRC_OPTIMAL�ɕϊ�
	vk::ImageMemoryBarrier barrier;
	barrier.oldLayout = vk::ImageLayout::eUndefined;
	barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
	barrier.image = srcImage->GetImageBuffer();
	barrier.subresourceRange.aspectMask = srcImage->GetAspectFlag();
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = vk::AccessFlagBits::eMemoryRead;
	barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
		vk::PipelineStageFlagBits::eTransfer,
		{}, {}, {}, barrier);


	// �X�e�[�W���O�o�b�t�@����C���[�W�փR�s�[
	vk::BufferImageCopy copyRegion;
	copyRegion.bufferOffset = 0;
	copyRegion.bufferRowLength = 0;			// "0"���w�肵�Ă����Ǝ����I��Extent�̒l������
	copyRegion.bufferImageHeight = 0;		// "0"���w�肵�Ă����Ǝ����I��Extent�̒l������
	copyRegion.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
	copyRegion.imageSubresource.mipLevel = 0;
	copyRegion.imageSubresource.baseArrayLayer = 0;
	copyRegion.imageSubresource.layerCount = 1;
	copyRegion.imageOffset = vk::Offset3D{ 0, 0, 0 };
	copyRegion.imageExtent = vk::Extent3D{ imageWidth, imageHeight, 1 };

	commandBuffer.copyImageToBuffer(
		srcImage->GetImageBuffer(), vk::ImageLayout::eTransferSrcOptimal,
		dstBuffer,
		{ copyRegion });


	// ���C�A�E�g�����ɖ߂�
	barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
	barrier.newLayout = vk::ImageLayout::eColorAttachmentOptimal;
	barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
	barrier.dstAccessMask = vk::AccessFlagBits::eMemoryRead;

	commandBuffer.pipelineBarrier(
		vk::PipelineStageFlagBits::eTransfer,
		vk::PipelineStageFlagBits::eFragmentShader,
		{}, {}, {}, barrier);

	// �R�}���h�o�b�t�@�̏I��
	commandBuffer.end();

}

