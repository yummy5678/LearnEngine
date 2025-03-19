#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include "MeshObject.h"
#include "StagingImageBuffer.h"
#include "ImageSet.h"

class VImageBufferBase
{
public:
	VImageBufferBase(
		vk::ImageUsageFlags bufferusage,			// �摜�̎g�p�p�r 
		vk::SharingMode	sharingMode,				// �摜�̃L���[�Ԃł̋��L�ݒ�
		vk::ImageAspectFlags aspectFlag,			// �摜�̃A�X�y�N�g�t���O
		vk::Format format,							// �摜�̃t�H�[�}�b�g
		VkMemoryPropertyFlags requiredFlag,			// �g�p����o�b�t�@�̕K�{�v��
		VkMemoryPropertyFlags preferredFlag,		// �g�p����o�b�t�@�̗D��v��
		VmaAllocationCreateFlags allocationFlag);	// �������̊��蓖�ĕ���
	~VImageBufferBase();

	//void SetImage(VmaAllocator* allocator, uint32_t textureWidth, uint32_t textureHeight);
	vk::Image GetImageBuffer();
	vk::ImageView GetImageView();
	ImageSet GetImageSet();

	void Cleanup();




protected:
	VmaAllocator* m_pAllocator;


	// �o�b�t�@�̃^�C�v
	vk::ImageUsageFlags		m_Usage;
	// �L���[�Ԃ̓ǂݎ��ݒ�
	vk::SharingMode			m_SharingMode;
	vk::Format				m_Format;
	vk::ImageAspectFlags	m_AspectFlag;

	ImageSet m_ImageSet;	// �C���[�W�ƃC���[�W���̃Z�b�g

	VmaAllocation			m_ImageAllocation;	// �f�[�^�o�b�t�@�̗̈�	

	VmaAllocationCreateFlags	m_AllocationFlag;	// �������̊��蓖�ĕ���
	VkMemoryPropertyFlags		m_RequiredFlag;		// �������̕K�{����
	VkMemoryPropertyFlags		m_PreferredFlag;	// �������̗D�����

	VkImageCreateInfo CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight);
	VkImageCreateInfo CreateImageInfo(vk::Extent3D extent);
	void CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight);
	void CreateBuffer(VmaAllocator* allocator, vk::Extent2D extent);
	void CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag);


	// �f�[�^���o�b�t�@�ɏ�������
	void MapData(void* setData, vk::DeviceSize dataSize);
};

