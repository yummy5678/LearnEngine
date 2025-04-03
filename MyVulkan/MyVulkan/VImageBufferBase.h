#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include "NonCopyable.h"
#include "MeshObject.h"
#include "StagingImageBuffer.h"
#include "ImageSet.h"

class VImageBufferBase : public NonCopyable
{
public:
	VImageBufferBase(
		vk::ImageUsageFlags		bufferusage,		// �摜�̎g�p�p�r 
		vk::SharingMode			sharingMode,		// �摜�̃L���[�Ԃł̋��L�ݒ�
		vk::ImageAspectFlags	aspectFlag,			// �摜�̃A�X�y�N�g�t���O
		vk::Format				format,				// �摜�̃t�H�[�}�b�g
		vk::MemoryPropertyFlags requiredFlag,		// �g�p����o�b�t�@�̕K�{�v��
		vk::MemoryPropertyFlags preferredFlag,		// �g�p����o�b�t�@�̗D��v��
		VmaAllocationCreateFlags allocationFlag);	// �������̊��蓖�ĕ���
	~VImageBufferBase();

	//void SetImage(VmaAllocator* allocator, uint32_t textureWidth, uint32_t textureHeight);
	vk::Image GetImageBuffer();
	vk::ImageView GetImageView();
	ImageSet GetImageSet();
	vk::Extent3D GetExtent();

	vk::ImageUsageFlags GetUsage();
	vk::SharingMode			GetSharingMode();
	vk::Format GetFormat();
	vk::ImageAspectFlags GetAspectFlag();

	void Cleanup();

	// �쐬���ɓo�^�����A���P�[�^�[��Ԃ�
	VmaAllocator* GetUsingAllocator();


protected:
	VmaAllocator* m_pAllocator;


	// �o�b�t�@�̃^�C�v
	vk::ImageUsageFlags		m_Usage;
	// �L���[�Ԃ̓ǂݎ��ݒ�
	vk::SharingMode			m_SharingMode;
	vk::Format				m_Format;
	vk::ImageAspectFlags	m_AspectFlag;
	vk::Extent3D			m_Extent;

	ImageSet m_ImageSet;	// �C���[�W�ƃC���[�W���̃Z�b�g

	VmaAllocation			m_ImageAllocation;	// �f�[�^�o�b�t�@�̗̈�	

	VmaAllocationCreateFlags	m_AllocationFlag;	// �������̊��蓖�ĕ���
	vk::MemoryPropertyFlags		m_RequiredFlag;		// �������̕K�{����
	vk::MemoryPropertyFlags		m_PreferredFlag;	// �������̗D�����

	VkImageCreateInfo CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight);
	VkImageCreateInfo CreateImageInfo(vk::Extent3D extent);
	void CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight);
	void CreateBuffer(VmaAllocator* allocator, vk::Extent2D extent);
	void CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag);


	// �f�[�^���o�b�t�@�ɏ�������
	/*void MapData(void* setData, vk::DeviceSize dataSize);*/
};

