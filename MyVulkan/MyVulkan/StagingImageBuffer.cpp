#include "StagingImageBuffer.h"

VStagingImageBuffer::VStagingImageBuffer() :
	VBufferBase(stagingImageUsage)
{
}

VStagingImageBuffer::~VStagingImageBuffer()
{
}

void VStagingImageBuffer::Initialize(VmaAllocator allocator, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageChannel)
{
	m_Allocator = allocator;
	m_LogicalDevice = vk::Device(allocator->m_hDevice);
	m_PhysicalDevice = vk::PhysicalDevice(allocator->GetPhysicalDevice());
	m_ImageWidth = imageWidth;
	m_ImageHeight = imageHeight;
	m_ImageChannel = imageChannel;

	uint32_t dataSize = imageWidth * imageHeight * imageChannel;

	// �]���p�L���[�̎擾
	QueueFamilySelector queueFamily(m_PhysicalDevice);
	m_CommandPool = CreateCommandPool(m_LogicalDevice, queueFamily.GetTransferIndex());
	m_CommandBuffer = CreateCommandBuffer(m_LogicalDevice, m_CommandPool);
	m_Queue = m_LogicalDevice.getQueue(queueFamily.GetTransferIndex(), 0);

	auto stagingBufferInfo = CreateBufferInfo(dataSize, m_Usage, m_SharingMode);


	// CPU����GPU�֏��𑗂�̂ɓK�����������̈���쐬������
	VmaAllocationCreateInfo stagingAllocateInfo;
	stagingAllocateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;	// �z�X�g����̏������݂�����
	stagingAllocateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;						// CPU����A�N�Z�X�\


	// �X�e�[�W���O�o�b�t�@�̍쐬
	auto result = vmaCreateBuffer(allocator, &stagingBufferInfo, &stagingAllocateInfo, &m_Buffer, &m_Allocation, nullptr);
	// �X�e�[�W���O�o�b�t�@�ƃ������̍쐬
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("�X�e�[�W���O�o�b�t�@�̍쐬�Ɏ��s���܂���!");
	}

}

void VStagingImageBuffer::TransferDataToImageBuffer(void* transfarData, vk::Image toBuffer)
{
	if (!m_Allocator) throw std::runtime_error("��ɃX�e�[�W���O�o�b�t�@�̏��������s���Ă�������!");

	// �f�[�^���X�e�[�W���O�o�b�t�@�ɃR�s�[
	vk::DeviceSize dataSize = m_ImageWidth * m_ImageHeight * m_ImageChannel;
	MapData(m_Allocator, transfarData, dataSize);

	// �]���p�o�b�t�@�̃f�[�^������̃C���[�W�o�b�t�@�ɃR�s�[
	SetCopyToImageCommand(m_CommandBuffer, m_Buffer, toBuffer, m_ImageWidth, m_ImageHeight);	// �]���R�}���h���쐬

	// �R�}���h�o�b�t�@�����s
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;				// �g���R�}���h��1�����ŏ[��
	submitInfo.pCommandBuffers = &m_CommandBuffer;	// �쐬�����R�}���h�o�b�t�@���Z�b�g

	m_Queue.submit(1, &submitInfo, nullptr);		// �R�}���h��GPU�̃L���[�ɑ��M
	m_Queue.waitIdle();								// �������R�}���h�L���[�̊�����҂�
}

vk::CommandPool VStagingImageBuffer::CreateCommandPool(vk::Device logicalDevice, uint32_t queueFamilyIndex)
{
	// �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
	vk::CommandPoolCreateInfo poolInfo;
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

	// �R�}���h�o�b�t�@�̏I��
	commandBuffer.end();
}

void VStagingImageBuffer::MapData(VmaAllocator allocator, void* setData, vk::DeviceSize dataSize)
{
	// �m�ۂ����o�b�t�@�̗̈�̃|�C���^���擾
	void* mapData;
	vmaMapMemory(allocator, m_Allocation, &mapData);

	// ���_�f�[�^�̏����擾�����o�b�t�@�ɃR�s�[
	memcpy(mapData, setData, dataSize);

	// �������̃A�N�Z�X����������
	vmaUnmapMemory(allocator, m_Allocation);

}
