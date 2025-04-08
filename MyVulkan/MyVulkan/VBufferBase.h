#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include <cstring>
#include "NonCopyable.h"

// �o�b�t�@�쐬�p�̊��N���X
// ���_�o�b�t�@�N���X�Ȃǂɔh�������Ďg���\��


// GPU�ǂݎ��p�̃t���O
//constexpr vk::BufferUsageFlags localUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

class VBufferBase : public NonCopyable
{
public:
	VBufferBase(vk::BufferUsageFlags bufferusage,	// �o�b�t�@�̎g�p�p�r
		VkMemoryPropertyFlags requiredFlag,			// �g�p����o�b�t�@�̕K�{�v��
		VkMemoryPropertyFlags preferredFlag,		// �g�p����o�b�t�@�̗D��v��
		VmaAllocationCreateFlags allocationFlag);	// �������̊��蓖�ĕ���
	~VBufferBase();

	virtual vk::Buffer		GetBuffer();
	virtual vk::DeviceSize	GetDataSize();

	// �쐬���ɓo�^�����A���P�[�^�[��Ԃ�
	virtual const VmaAllocator* GetUsingAllocator();
	
	virtual void Cleanup();

protected:
	// ���т����̃f�o�C�X
	VmaAllocator*				m_pAllocator;

	// �o�b�t�@�̎g�p�p�r
	vk::BufferUsageFlags		m_BufferUsage;
	// �L���[�Ԃ̓ǂݎ��ݒ�
	vk::SharingMode				m_SharingMode = vk::SharingMode::eExclusive;

	//VmaMemoryUsage				m_MemoryUsage = VMA_MEMORY_USAGE_AUTO;
	VmaAllocationCreateFlags	m_AllocationFlag;	// �������̊��蓖�ĕ���
	VkMemoryPropertyFlags		m_RequiredFlag;		// �������̕K�{����
	VkMemoryPropertyFlags		m_PreferredFlag;	// �������̗D�����

	VkBuffer					m_Buffer;
	VmaAllocation				m_Allocation;

	vk::DeviceSize				m_DataSize;
	VmaAllocationInfo			m_AllocationInfo;

	void CreateBuffer(VmaAllocator* allocator, vk::DeviceSize dataSize);

	// �o�b�t�@�̍쐬�֐�
	VkBufferCreateInfo CreateBufferInfo(vk::DeviceSize dataSize, vk::BufferUsageFlags usage, vk::SharingMode mode);

	// ���������蓖�Ċ֐�
	uint32_t	FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags findType);

	// �f�[�^���o�b�t�@�ɏ�������
	void MapData(void* dstData, void* setData);

};

// �g�����X�t�@�[�o�b�t�@�ɂ���