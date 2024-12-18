#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>

class VImage
{
public:
	VImage();
	~VImage();

	void Create(VmaAllocator* allocator, vk::ImageCreateInfo createInfo, vk::ImageAspectFlags aspectFlag);
	void Cleanup();

	vk::Image		GetImage();
	vk::ImageView	GetImageView();

private:
	VmaAllocator* m_Allocator;

	vk::Image				m_Image;			// �C���[�W�o�b�t�@
	vk::ImageCreateInfo		m_ImageCreateInfo;	// �C���[�W�쐬���
	vk::ImageView			m_ImageView;		// �C���[�W�r���[
	VmaAllocation			m_ImageAllocation;	// �f�[�^�o�b�t�@�̗̈�


	void CreateBuffer(VmaAllocator* allocator, vk::ImageCreateInfo createInfo);
	void CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag);

};

