#include "StagingBuffer.h"

VStagingBuffer::VStagingBuffer() :
	VBufferBase(stagingUsage,
		VMA_MEMORY_USAGE_AUTO_PREFER_HOST)
{
}

VStagingBuffer::~VStagingBuffer()
{
}

void VStagingBuffer::Initialize(VmaAllocator allocator, vk::DeviceSize dataSize)
{
	m_Allocator = allocator;
	m_LogicalDevice		= vk::Device(allocator->m_hDevice);
	m_PhysicalDevice	= vk::PhysicalDevice(allocator->GetPhysicalDevice());

	m_BufferDataSize = dataSize;

	// Get indices of queue families from device
	QueueFamilySelector queueFamily(m_PhysicalDevice);
	m_CommandPool		= CreateCommandPool(m_LogicalDevice, queueFamily.GetTransferIndex());
	m_CommandBuffer		= CreateCommandBuffer(m_LogicalDevice, m_CommandPool);
	// �O���t�B�b�N�X�L���[�̎擾
	m_Queue = m_LogicalDevice.getQueue(queueFamily.GetTransferIndex(), 0);

	auto stagingBufferInfo = CreateBufferInfo(dataSize, m_BufferUsage, m_SharingMode);


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

void VStagingBuffer::TransferDataToBuffer(void* transfarData, vk::Buffer toBuffer)
{
	if(!m_Allocator) throw std::runtime_error("��ɃX�e�[�W���O�o�b�t�@�̏��������s���Ă�������!");

	// �f�[�^���X�e�[�W���O�o�b�t�@�ɃR�s�[
	MapData(m_Allocator, transfarData, m_BufferDataSize);

	// �g�����X�t�@�[�o�b�t�@�̃f�[�^������̃o�b�t�@�ɃR�s�[
	SetCopyBufferCommand(m_CommandBuffer, m_Buffer, toBuffer, m_BufferDataSize);	// �]���R�}���h���쐬

	// �R�}���h�o�b�t�@�����s
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_CommandBuffer;

	m_Queue.submit(1, &submitInfo, nullptr);
	m_Queue.waitIdle(); // ������҂�
}

vk::CommandPool VStagingBuffer::CreateCommandPool(vk::Device logicalDevice, uint32_t queueFamilyIndex)
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

vk::CommandBuffer VStagingBuffer::CreateCommandBuffer(vk::Device logicalDevice, vk::CommandPool commandPool)
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

void VStagingBuffer::SetCopyBufferCommand(vk::CommandBuffer commandBuffer, vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
{
	// �R�}���h�o�b�t�@�̊J�n
	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
	commandBuffer.begin(beginInfo);

	// �R�s�[�R�}���h�̍쐬
	vk::BufferCopy copyRegion;
	copyRegion.srcOffset = 0; // ���M���I�t�Z�b�g
	copyRegion.dstOffset = 0; // ��M��I�t�Z�b�g
	copyRegion.size = size;    // �]���T�C�Y
	commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);

	// �R�}���h�o�b�t�@�̏I��
	commandBuffer.end();
}


