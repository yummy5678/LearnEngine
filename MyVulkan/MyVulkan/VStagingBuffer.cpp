#include "VStagingBuffer.h"

VStagingBuffer::VStagingBuffer() :
	VBufferBase(stagingUsage)
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
	m_CommandPool		= CreateCommandPool(m_LogicalDevice, m_PhysicalDevice);
	m_CommandBuffer		= CreateCommandBuffer(m_LogicalDevice, m_CommandPool);

	auto stagingBufferInfo = CreateBufferInfo(dataSize, m_DataUsage);


	// CPU����GPU�֏��𑗂�̂ɓK�����������̈���쐬������
	VmaAllocationCreateInfo stagingAllocateInfo;
	stagingAllocateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;	// �z�X�g����̏������݂�����
	stagingAllocateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;						// CPU����A�N�Z�X�\


	// �X�e�[�W���O�o�b�t�@�̍쐬
	auto result = vmaCreateBuffer(allocator, &stagingBufferInfo, &stagingAllocateInfo, &m_DataBuffer, &m_DataAllocation, nullptr);
	// �X�e�[�W���O�o�b�t�@�ƃ������̍쐬
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("�X�e�[�W���O�o�b�t�@�̍쐬�Ɏ��s���܂���!");
	}



}

void VStagingBuffer::TransferDataToBuffer(void* transfarData, vk::DeviceSize dataSize, vk::Buffer toBuffer)
{
	if(!m_Allocator) throw std::runtime_error("��ɃX�e�[�W���O�o�b�t�@�̃C�j�V�����C�Y���s���Ă�������!");

	// �f�[�^���X�e�[�W���O�o�b�t�@�ɃR�s�[
	MapData(m_Allocator, transfarData, dataSize);

	// �g�����X�t�@�[�o�b�t�@�̃f�[�^������̃o�b�t�@�ɃR�s�[



}

vk::CommandPool VStagingBuffer::CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
{
	// Get indices of queue families from device
	QueueFamilySelector queue(physicalDevice);

	// �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.queueFamilyIndex = queue.GetTransferIndex();
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
	allocInfo.level = vk::CommandBufferLevel::ePrimary;  // �v���C�}���R�}���h�o�b�t�@
	allocInfo.commandBufferCount = 1;  // 1�̃R�}���h�o�b�t�@�����蓖��

	try
	{
		return logicalDevice.allocateCommandBuffers(allocInfo)[0];
	}
	catch (const std::exception&)
	{
		throw std::runtime_error("�]���p�R�}���h�o�b�t�@�̍쐬�Ɏ��s���܂����I");
	}
	
}
