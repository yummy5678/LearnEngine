#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>


class VmaCreator
{
public:
	VmaCreator();
	~VmaCreator();

	void CreateAllocator(vk::Instance instance, vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);
	VmaAllocator GetAllocator();

private:
	VmaAllocator m_Allocator;
	VkAllocationCallbacks m_Callbacks;

	void CreateCallBacks();



};

