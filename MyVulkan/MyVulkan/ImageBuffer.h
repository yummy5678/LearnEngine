#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include "MeshObject.h"
#include "StagingImageBuffer.h"

// RAM�ɒu����Ă���摜�f�[�^��VRAM�Ɉڍs����N���X
class VTextureBuffer
{
public:
	VTextureBuffer();
	~VTextureBuffer();
	void Cleanup();

	void SetImage(VmaAllocator* allocator, Texture& texture);

	vk::Image GetImageBuffer();

	vk::ImageView GetImageView();
	std::vector<VkDescriptorSet> GetDescripterSets();
protected:
	VmaAllocator*			m_Allocator;


	// �o�b�t�@�̃^�C�v
	vk::ImageUsageFlags		m_Usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled; // �]����&�T���v�����O�Ɏg�p
	// �L���[�Ԃ̓ǂݎ��ݒ�
	vk::SharingMode			m_SharingMode = vk::SharingMode::eExclusive;
	vk::Format				m_Format = vk::Format::eR8G8B8A8Unorm;
	vk::ImageAspectFlags	m_AspectFlag = vk::ImageAspectFlagBits::eColor;

	vk::Image				m_Buffer;			// �C���[�W�o�b�t�@
	vk::ImageView			m_ImageView;		// �C���[�W�r���[
	VmaAllocation			m_ImageAllocation;	// �f�[�^�o�b�t�@�̗̈�
	

	VkImageCreateInfo CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight, vk::Format format, vk::ImageUsageFlags usage, vk::SharingMode mode);
	void CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight);
	void CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag);
	void CreateSampler();
	void CreateDescripterSets(vk::ImageLayout imageLayout, vk::Sampler sampler);

};

