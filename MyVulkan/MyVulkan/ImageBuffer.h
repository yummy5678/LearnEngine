#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include "MeshObject.h"
#include "StagingBuffer.h"

// RAM�ɒu����Ă���摜�f�[�^��VRAM�Ɉڍs����N���X
class VImageBuffer
{
public:
	VImageBuffer();
	~VImageBuffer();


	void SetImage(VmaAllocator allocator, Texture& texture, vk::Format format);

	vk::Image GetImage();

protected:
	// ���т����̃f�o�C�X
	VmaAllocator			m_Allocator;

	// �o�b�t�@�̃^�C�v
	vk::BufferUsageFlags	m_Usage;

	// �L���[�Ԃ̓ǂݎ��ݒ�
	vk::SharingMode			m_SharingMode = vk::SharingMode::eExclusive;

	vk::Image				m_Buffer;


};

