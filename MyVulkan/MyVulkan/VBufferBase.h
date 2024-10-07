#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>

// �o�b�t�@�쐬�p�̊��N���X
// ���_�o�b�t�@�N���X�Ȃǂɔh�������Ďg���\��



// ���M�p�o�b�t�@�ɐݒ肷��\��̃t���O
constexpr vk::BufferUsageFlags stagingUsage = vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst;
// GPU�ǂݎ��p�̃t���O
//constexpr vk::BufferUsageFlags localUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

class VBufferBase
{
public:
	VBufferBase(vk::BufferUsageFlags usage);
	~VBufferBase();

	void SetData(void* pData,vk::DeviceSize dataSize);
	
	void Cleanup();

protected:
	// ���т����̃f�o�C�X
	//vk::Device				m_LogicalDevice;
	//vk::PhysicalDevice		m_PhysicalDevice;
	VmaAllocator				m_Allocator;

	// �o�b�t�@�̃^�C�v
	vk::BufferUsageFlags	m_DataUsage;
	// �L���[�Ԃ̓ǂݎ��ݒ�
	vk::SharingMode			m_SharingMode = vk::SharingMode::eExclusive;

	VkBuffer        m_DataBuffer;
	VmaAllocation	m_DataAllocation;

	VkBuffer        m_StagingBuffer;
	VmaAllocation	m_StagingAllocation;

	void CreateBuffer(VmaAllocator allocator, vk::Buffer buffer, vk::DeviceSize dataSize);

	// �o�b�t�@�̍쐬�֐�
	VkBufferCreateInfo CreateBufferInfo(vk::DeviceSize dataSize, vk::BufferUsageFlags usage);

	// ���������蓖�Ċ֐�
	void		AllocateBufferMemory(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags propertyFlags);
	uint32_t	FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags findType);

	// �f�[�^���X�e�[�W���O�o�b�t�@�ɃR�s�[
	void MapData(VmaAllocator allocator, void* pData, vk::DeviceSize dataSize);

};

// �g�����X�t�@�[�o�b�t�@�ɂ���