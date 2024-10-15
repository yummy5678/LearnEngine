#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>

// RAM�ɒu����Ă���摜�f�[�^��VRAM�Ɉڍs����N���X
class VImageBuffer
{
public:
	VImageBuffer();
	~VImageBuffer();


	void createImage(VmaAllocation& allocation, void* data, uint32_t width, uint32_t height, vk::Format format);


protected:
	// ���т����̃f�o�C�X
	VmaAllocator				m_Allocator;

	// �o�b�t�@�̃^�C�v
	vk::BufferUsageFlags	m_Usage;

	// �L���[�Ԃ̓ǂݎ��ݒ�
	vk::SharingMode			m_SharingMode = vk::SharingMode::eExclusive;

	vk::Image       m_Buffer;
	VmaAllocation	m_Allocation;


};

