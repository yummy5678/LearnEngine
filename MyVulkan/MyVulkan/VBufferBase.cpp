#include "VBufferBase.h"

VBufferBase::VBufferBase(vk::BufferUsageFlags usage)
{
	//m_DataUsage�̓X�e�[�W���O�o�b�t�@����f�[�^��ݒ肷�����Ȃ̂œ]����t���O��ǉ�
	m_Usage = usage | vk::BufferUsageFlagBits::eTransferDst;  
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

	auto dataBufferInfo = CreateBufferInfo(dataSize, m_Usage);


	// CPU����GPU�֏��𑗂�̂ɓK�����������̈���쐬������
	VmaAllocationCreateInfo dataAllocateInfo;
	dataAllocateInfo.usage = VMA_MEMORY_USAGE_AUTO;	// �����ōœK�ȃ�������I��(�ʏ��GPU���[�J��������)



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
VkBufferCreateInfo VBufferBase::CreateBufferInfo(vk::DeviceSize dataSize, vk::BufferUsageFlags usage)
{
	vk::BufferCreateInfo bufferCreateInfo;
	bufferCreateInfo.pNext;
	bufferCreateInfo.flags;
	bufferCreateInfo.size					= dataSize;
	bufferCreateInfo.usage					= usage;			// ���_�o�b�t�@�Ƃ��Ďg�p
	bufferCreateInfo.sharingMode			= m_SharingMode;	// �o�b�t�@�̎g�p��1�̃L���[�Ɍ���
	bufferCreateInfo.queueFamilyIndexCount;
	bufferCreateInfo.pQueueFamilyIndices;

	// �_���f�o�C�X��ʂ��ăo�b�t�@���쐬
	return bufferCreateInfo;
}

//void VBufferBase::AllocateBufferMemory(VmaAllocator allocator, vk::Buffer buffer, vk::MemoryPropertyFlags propertyFlags)
//{
//
//
//	// �C���[�W�̃������v�����擾
//	vk::MemoryRequirements memoryRequirements = logicalDevice.getBufferMemoryRequirements(buffer);
//
//	// �T���Ă��郁�����v���p�e�B�̃t���O
//	vk::MemoryPropertyFlags properties = propertyFlags;
//
//	// �������̊��蓖�ď���ݒ�
//	//vk::MemoryAllocateInfo allocateInfo;
//	//allocateInfo.pNext;
//	//allocateInfo.allocationSize = memoryRequirements.size;  // �摜�̃������T�C�Y
//	//allocateInfo.memoryTypeIndex = FindMemoryType(logicalDevice, physicalDevice, buffer, properties);   // �������^�C�v
//
//	VmaAllocationCreateInfo allocateInfo;
//	allocateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU; // CPU����GPU�ւ̓]���p
//
//
//	auto result = vmaCreateBuffer(allocator, &bufferInfo, &allocateInfo, &m_TransfarBuffer, &vertexBufferAllocation, nullptr);
//
//	// �o�b�t�@�ƃ������̊��蓖��
//	if (result != VK_SUCCESS) {
//		throw std::runtime_error("�o�b�t�@�̍쐬�Ɏ��s���܂���!");
//	}
//
//	return allocateInfo;
//}

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




