#include "VBufferBase.h"
#include "GraphicsDefine.h"
#include <iostream>

VBufferBase::VBufferBase(vk::BufferUsageFlags bufferusage,
	VkMemoryPropertyFlags requiredFlag,
	VkMemoryPropertyFlags preferredFlag,
	VmaAllocationCreateFlags allocationFlag) :
	m_pAllocator(VK_NULL_HANDLE),
	m_Buffer(VK_NULL_HANDLE),
	m_Allocation(VK_NULL_HANDLE),
	m_RequiredFlag(requiredFlag),
	m_PreferredFlag(preferredFlag),
	m_DataSize(0),
	m_AllocationFlag(allocationFlag)
{
	// m_DataUsage�̓X�e�[�W���O�o�b�t�@����f�[�^��ݒ肷�����Ȃ̂œ]����t���O��ǉ�
	m_BufferUsage = bufferusage | vk::BufferUsageFlagBits::eTransferDst;
};

VBufferBase::~VBufferBase()
{
	//Cleanup();
}


vk::Buffer VBufferBase::GetBuffer()
{
	return vk::Buffer(m_Buffer);
}

vk::DeviceSize VBufferBase::GetDataSize()
{
	return m_DataSize;
}

void VBufferBase::Cleanup()
{
	if (!m_pAllocator || !m_Buffer) return;

	vmaDestroyBuffer(*m_pAllocator, m_Buffer, m_Allocation);
	m_pAllocator = nullptr;
}

void VBufferBase::CreateBuffer(VmaAllocator* allocator, vk::DeviceSize dataSize)
{
	//m_pAllocator = allocator;
	m_DataSize = dataSize;

	auto dataBufferInfo = CreateBufferInfo(dataSize, m_BufferUsage, m_SharingMode);

	VmaAllocationCreateInfo dataAllocateInfo;
	dataAllocateInfo.priority = 1.0f;					// �D��x
	dataAllocateInfo.flags = m_AllocationFlag;			// �������̊��蓖�ĕ���
	dataAllocateInfo.requiredFlags  = m_RequiredFlag;	// �������̕K�{����
	dataAllocateInfo.preferredFlags = m_PreferredFlag;	// �������̗D�����
	dataAllocateInfo.pool = VK_NULL_HANDLE;				// VMA�Ɍ��߂�����̂�NULL
	dataAllocateInfo.memoryTypeBits = NULL;				// VMA�Ɍ��߂�����̂�NULL
	dataAllocateInfo.pUserData = nullptr;				// �ǉ��œn�����͍��̂Ƃ��떳���̂�null
	dataAllocateInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;	// �g�p���Ȃ��Ȃ����l


	// GPU���Ŏg�����_�o�b�t�@�̍쐬
	bool result = vmaCreateBuffer(*allocator, &dataBufferInfo, &dataAllocateInfo, &m_Buffer, &m_Allocation, nullptr);
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

void VBufferBase::MapData(void* setData)
{
	VkResult mappingResult;

	// �m�ۂ����o�b�t�@�̗̈�̃|�C���^���擾
	// VMA�Ŋm�ۂ�����������CPU����A�N�Z�X�\�Ƀ}�b�v����
	void* mapData = nullptr;
	mappingResult = vmaMapMemory(*m_pAllocator, m_Allocation, &mapData);

	// �}�b�s���O�Ɏ��s�����ꍇ�̃G���[�`�F�b�N
	if (mappingResult != VK_SUCCESS) 
	{
		std::cerr << "�������[�}�b�s���O�Ɏ��s���܂���!" << std::endl;
		return;
	}

	// �}�b�v���ꂽ�������̈�ɁAsetData����m_DataSize�o�C�g���̃f�[�^���R�s�[����
	std::memcpy(mapData, setData, m_DataSize);


	// �������̃A�N�Z�X����������
	vmaUnmapMemory(*m_pAllocator, m_Allocation);
}




