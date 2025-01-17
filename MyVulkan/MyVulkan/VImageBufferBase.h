#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include "MeshObject.h"
#include "StagingImageBuffer.h"



class VImageBufferBase
{
public:
	VImageBufferBase(vk::ImageUsageFlags bufferusage, 
		VkMemoryPropertyFlags requiredFlag,			// �g�p����o�b�t�@�̕K�{�v��
		VkMemoryPropertyFlags preferredFlag,		// �g�p����o�b�t�@�̗D��v��
		VmaAllocationCreateFlags allocationFlag);	// �������̊��蓖�ĕ���
	~VImageBufferBase();

	//void SetImage(VmaAllocator* allocator, uint32_t textureWidth, uint32_t textureHeight);
	vk::Image GetImageBuffer();
	vk::ImageView GetImageView();

	void Cleanup();




protected:
	VmaAllocator* m_pAllocator;


	// �o�b�t�@�̃^�C�v
	vk::ImageUsageFlags		m_Usage;
	// �L���[�Ԃ̓ǂݎ��ݒ�
	vk::SharingMode			m_SharingMode;
	vk::Format				m_Format;
	vk::ImageAspectFlags	m_AspectFlag;

	vk::Image				m_ImageBuffer;			// �C���[�W�o�b�t�@
	vk::ImageView			m_ImageView;		// �C���[�W�r���[
	VmaAllocation			m_ImageAllocation;	// �f�[�^�o�b�t�@�̗̈�	

	VmaAllocationCreateFlags	m_AllocationFlag;	// �������̊��蓖�ĕ���
	VkMemoryPropertyFlags		m_RequiredFlag;		// �������̕K�{����
	VkMemoryPropertyFlags		m_PreferredFlag;	// �������̗D�����

	VkImageCreateInfo CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight);
	void CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight);
	void CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag);


	// �f�[�^���o�b�t�@�ɏ�������
	void MapData(void* setData, vk::DeviceSize dataSize);
};

