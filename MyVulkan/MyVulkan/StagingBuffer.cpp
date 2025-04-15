#include "StagingBuffer.h"


VStagingBuffer::VStagingBuffer() :
	VBufferBase(
		vk::BufferUsageFlagBits::eTransferSrc | 				// �o�b�t�@�̎g�p�p�r
		vk::BufferUsageFlagBits::eTransferDst,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |					// �g�p����o�b�t�@�̕K�{�v��		
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_MEMORY_PROPERTY_HOST_CACHED_BIT,						// �g�p����o�b�t�@�̗D��v��
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |// �������̊��蓖�ĕ���
		VMA_ALLOCATION_CREATE_MAPPED_BIT),
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
	m_CommandBuffer(VK_NULL_HANDLE),
	m_CommandPool(VK_NULL_HANDLE),
	m_Queue(VK_NULL_HANDLE)
{
}

VStagingBuffer::~VStagingBuffer()
{
	VStagingBuffer::Cleanup();
}

void VStagingBuffer::Initialize(VmaAllocator* allocator, vk::DeviceSize dataSize)
{
	m_pAllocator = allocator;

	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);
	m_LogicalDevice		= vk::Device(allocatorInfo.device);
	m_PhysicalDevice	= vk::PhysicalDevice(allocatorInfo.physicalDevice);

	//m_DataSize = dataSize;


	QueueFamilySelector queueFamily;
	queueFamily.Initialize(m_PhysicalDevice);
	m_CommandPool		= CreateCommandPool(m_LogicalDevice, queueFamily.GetTransferIndex());
	m_CommandBuffer		= CreateCommandBuffer(m_LogicalDevice, m_CommandPool);
	// �O���t�B�b�N�X�L���[�̎擾
	m_Queue = m_LogicalDevice.getQueue(queueFamily.GetTransferIndex(), 0);
	



	VBufferBase::CreateBuffer(allocator, dataSize);

	//auto stagingBufferInfo = CreateBufferInfo(dataSize, m_BufferUsage, m_SharingMode);

	//// CPU����GPU�֏��𑗂�̂ɓK�����������̈���쐬������
	//VmaAllocationCreateInfo stagingAllocateInfo;
	//stagingAllocateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;	// �z�X�g����̏������݂�����
	//stagingAllocateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;						// CPU����A�N�Z�X�\

	//// �X�e�[�W���O�o�b�t�@�̍쐬
	//auto result = vmaCreateBuffer(*allocator, &stagingBufferInfo, &stagingAllocateInfo, &m_Buffer, &m_Allocation, nullptr);
	//// �X�e�[�W���O�o�b�t�@�ƃ������̍쐬
	//if (result != VK_SUCCESS)
	//{
	//	throw std::runtime_error("�X�e�[�W���O�o�b�t�@�̍쐬�Ɏ��s���܂���!");
	//}
}

void VStagingBuffer::TransferDataToBuffer(void* transfarData, vk::Buffer toBuffer, vk::Fence fence)
{
	if(!m_pAllocator) throw std::runtime_error("��ɃX�e�[�W���O�o�b�t�@�̏��������s���Ă�������!\n");

	// �f�[�^���X�e�[�W���O�o�b�t�@�ɃR�s�[
	MapData(m_AllocationInfo.pMappedData , transfarData);

	// �g�����X�t�@�[�o�b�t�@�̃f�[�^������̃o�b�t�@�ɃR�s�[
	SetCopyBufferCommand(m_CommandBuffer, m_Buffer, toBuffer, m_DataSize);	// �]���R�}���h���쐬

	// �R�}���h�o�b�t�@�����s
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_CommandBuffer;

	if(fence != VK_NULL_HANDLE)
	{
		m_LogicalDevice.waitForFences(fence, true, MAX_WAIT_TIME); // �O��̎g�p������҂�
		m_LogicalDevice.resetFences(fence);                         // �t�F���X�𖢊�����Ԃɖ߂�
	}

	vk::Result result = m_Queue.submit(1, &submitInfo, fence);
	if (result != vk::Result::eSuccess)
	{
		throw std::runtime_error("�������Ԃ̃f�[�^�̈ړ��Ɏ��s���܂���\n");
	}


	if (fence == VK_NULL_HANDLE)
	{
		m_Queue.waitIdle(); // ������҂�
	}
	else
	{
		m_LogicalDevice.waitForFences(fence, true, MAX_WAIT_TIME);
	}
}

void VStagingBuffer::Cleanup()
{
	if (m_LogicalDevice == VK_NULL_HANDLE) return;

	printf("�X�e�[�W���O�o�b�t�@��������܂�\n");

	if (m_CommandPool != VK_NULL_HANDLE)
	{
		m_LogicalDevice.destroyCommandPool(m_CommandPool);
		m_CommandPool = VK_NULL_HANDLE;
		m_CommandBuffer = VK_NULL_HANDLE;
	}

	m_Queue = VK_NULL_HANDLE;
	m_PhysicalDevice = VK_NULL_HANDLE;
	m_LogicalDevice = VK_NULL_HANDLE;

	VBufferBase::Cleanup();
}

vk::CommandPool VStagingBuffer::CreateCommandPool(vk::Device logicalDevice, uint32_t queueFamilyIndex)
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
		throw std::runtime_error("�]���p�R�}���h�v�[���̍쐬�Ɏ��s���܂����I\n");
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
		throw std::runtime_error("�]���p�R�}���h�o�b�t�@�̍쐬�Ɏ��s���܂����I\n");
	}
	
}

void VStagingBuffer::SetCopyBufferCommand(vk::CommandBuffer commandBuffer, vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
{
	// �R�}���h�o�b�t�@�̊J�n
	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.pNext;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
	commandBuffer.begin(beginInfo);

	// �R�s�[�R�}���h�̍쐬
	vk::BufferCopy copyRegion;
	copyRegion.srcOffset = 0; // ���M���I�t�Z�b�g
	copyRegion.dstOffset = 0; // ��M��I�t�Z�b�g
	copyRegion.size = size;    // �]���T�C�Y
	commandBuffer.copyBuffer(srcBuffer, dstBuffer, { copyRegion });

	// �R�}���h�o�b�t�@�̏I��
	commandBuffer.end();
}


