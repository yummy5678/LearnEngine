#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>

// �o�b�t�@�쐬�p�̊��N���X
// ���_�o�b�t�@�N���X�Ȃǂɔh�������Ďg���\��


// GPU�ǂݎ��p�̃t���O
//constexpr vk::BufferUsageFlags localUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

class VBufferBase
{
public:
	VBufferBase(vk::BufferUsageFlags bufferusage, VmaMemoryUsage memoryUsage);
	~VBufferBase();

	void SetData(void* pData,vk::DeviceSize dataSize);
	vk::Buffer		GetBuffer();
	vk::DeviceSize	GetDataSize();
	
	void Cleanup();

protected:
	// ���т����̃f�o�C�X
	VmaAllocator*				m_Allocator;

	// �o�b�t�@�̎g�p�p�r
	vk::BufferUsageFlags		m_BufferUsage;
	// �L���[�Ԃ̓ǂݎ��ݒ�
	vk::SharingMode				m_SharingMode = vk::SharingMode::eExclusive;

	VmaMemoryUsage				m_MemoryUsage;

	VkBuffer					m_Buffer;
	VmaAllocation				m_Allocation;

	vk::DeviceSize				m_DataSize;

	void CreateBuffer(VmaAllocator* allocator, vk::DeviceSize dataSize);

	// �o�b�t�@�̍쐬�֐�
	VkBufferCreateInfo CreateBufferInfo(vk::DeviceSize dataSize, vk::BufferUsageFlags usage, vk::SharingMode mode);

	// ���������蓖�Ċ֐�
	uint32_t	FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags findType);

	// �f�[�^���o�b�t�@�ɏ�������
	void MapData(VmaAllocator* allocator, void* setData, vk::DeviceSize dataSize);

};

// �g�����X�t�@�[�o�b�t�@�ɂ���