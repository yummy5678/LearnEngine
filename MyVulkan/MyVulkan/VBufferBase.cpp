#include "VBufferBase.h"

VBufferBase::VBufferBase(vk::BufferUsageFlags bufferusage, VmaAllocationCreateFlags memoryFlag)
{
	// m_DataUsage�̓X�e�[�W���O�o�b�t�@����f�[�^��ݒ肷�����Ȃ̂œ]����t���O��ǉ�
	m_BufferUsage = bufferusage | vk::BufferUsageFlagBits::eTransferDst;
	m_MemoryUsage = memoryFlag;
}

VBufferBase::~VBufferBase()
{
	Cleanup();
}

void VBufferBase::SetData(void* pData, vk::DeviceSize dataSize)
{


}

vk::Buffer VBufferBase::GetBuffer()
{
	return vk::Buffer(m_Buffer);
}

void VBufferBase::Cleanup()
{
	if (!m_Allocator) return;
	vmaDestroyBuffer(m_Allocator, m_Buffer, m_Allocation);

}

void VBufferBase::CreateBuffer(VmaAllocator allocator, vk::DeviceSize dataSize)
{
	m_Allocator = allocator;

	auto dataBufferInfo = CreateBufferInfo(dataSize, m_BufferUsage, m_SharingMode);


	// CPU����GPU�֏��𑗂�̂ɓK�����������̈���쐬������
	VmaAllocationCreateInfo dataAllocateInfo;
	dataAllocateInfo.flags = m_MemoryFlag;	// �����ōœK�ȃ�������I��(�ʏ��GPU���[�J��������)



	// GPU���Ŏg�����_�o�b�t�@�̍쐬
	auto result = vmaCreateBuffer(allocator, &dataBufferInfo, &dataAllocateInfo, &m_Buffer, &m_Allocation, nullptr);
	// �X�e�[�W���O�o�b�t�@�ƃ������̍쐬
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("���[�J���o�b�t�@�̍쐬�Ɏ��s���܂���!");
	}

}

// �����f�o�C�X�p�̃o�b�t�@�̍쐬
// dataSize : �m�ۂ���f�[�^�̃T�C�Y
VkBufferCreateInfo VBufferBase::CreateBufferInfo(vk::DeviceSize dataSize, vk::BufferUsageFlags bufferUsage, vk::SharingMode mode)
{
	vk::BufferCreateInfo bufferCreateInfo;
	bufferCreateInfo.pNext;
	bufferCreateInfo.flags;
	bufferCreateInfo.size					= dataSize;
	bufferCreateInfo.usage					= bufferUsage;		
	bufferCreateInfo.sharingMode			= mode;		// �o�b�t�@�̎g�p��1�̃L���[�Ɍ���
	bufferCreateInfo.queueFamilyIndexCount;
	bufferCreateInfo.pQueueFamilyIndices;

	// �_���f�o�C�X��ʂ��ăo�b�t�@���쐬
	return bufferCreateInfo;
}

uint32_t VBufferBase::FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags findType)
{
	// �o�b�t�@�̃������v�����擾
	vk::MemoryRequirements memoryRequirements = logicalDevice.getBufferMemoryRequirements(buffer);
	auto typeFilter = memoryRequirements.memoryTypeBits;

	// �����f�o�C�X(GPU)���烁�����v���p�e�B���擾�B
	vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		// typeFilter�́A�r�b�g�}�X�N�Ń������^�C�v���t�B���^�����O���邽�߂̂��́B
		// �����ł́AtypeFilter�̃r�b�g��i�Ԗڂ̃������^�C�v�ɑΉ����邩�ǂ������m�F���Ă��܂��B
		// (1 << i)�́Ai�r�b�g�ڂ̃r�b�g��1�ɂ����r�b�g�}�X�N�ł��B
		// typeFilter & (1 << i) �́Ai�r�b�g�ڂ̃r�b�g�������Ă��邩���m�F���܂��B
		if ((typeFilter & (1 << i)) &&
			(memoryProperties.memoryTypes[i].propertyFlags & findType) == findType)
		{
			return i;
		}
	}

	// �G���[���b�Z�[�W
	throw std::runtime_error("�K�؂ȃ������^�C�v���������܂���ł���!");
}




