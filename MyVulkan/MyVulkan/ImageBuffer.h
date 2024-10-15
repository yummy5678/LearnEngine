#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include "MeshObject.h"
#include "StagingImageBuffer.h"

// RAM�ɒu����Ă���摜�f�[�^��VRAM�Ɉڍs����N���X
class VImageBuffer
{
public:
	VImageBuffer();
	~VImageBuffer();
	void Cleanup();

	void SetImage(VmaAllocator allocator, Texture& texture, vk::Format format);

	vk::Image GetImage();

protected:
	// �o�b�t�@�̃^�C�v
	vk::ImageUsageFlags		m_Usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled; // �]���您��уT���v�����O�Ɏg�p
	// �L���[�Ԃ̓ǂݎ��ݒ�
	vk::SharingMode			m_SharingMode = vk::SharingMode::eExclusive;
	vk::Format				m_Format = vk::Format::eR8G8B8A8Unorm;

	// ���т����̃f�o�C�X
	VmaAllocator			m_Allocator;

	vk::Image				m_Buffer;
	VmaAllocation			m_ImageAllocation;

	VkImageCreateInfo CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight, vk::Format format, vk::ImageUsageFlags usage, vk::SharingMode mode);
	void CreateBuffer(VmaAllocator allocator, uint32_t imageWidth, uint32_t imageHeight);
};

