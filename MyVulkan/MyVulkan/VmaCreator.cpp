#include "VmaCreator.h"

VmaCreator::VmaCreator()
{
}

VmaCreator::~VmaCreator()
{
}

void VmaCreator::CreateAllocator(vk::Instance instance, vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
{

	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.physicalDevice = physicalDevice;
	allocatorInfo.device = logicalDevice;
	allocatorInfo.instance = instance;
	allocatorInfo.flags = 0;
	allocatorInfo.preferredLargeHeapBlockSize = 0;
	allocatorInfo.pAllocationCallbacks = &m_Callbacks;
	allocatorInfo.pDeviceMemoryCallbacks = nullptr;
	allocatorInfo.pHeapSizeLimit = nullptr;
	allocatorInfo.pVulkanFunctions = nullptr;

	// アロケータの作成
	VkResult result = vmaCreateAllocator(&allocatorInfo, &m_Allocator);

	// 失敗時
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VMAアロケータの作成に失敗しました!");
	}
}

VmaAllocator VmaCreator::GetAllocator()
{
	return m_Allocator;
}

void VmaCreator::CreateCallBacks()
{

	m_Callbacks.pUserData = nullptr;
	m_Callbacks.pfnAllocation = nullptr;
	m_Callbacks.pfnReallocation = nullptr;
	m_Callbacks.pfnFree = nullptr;
	m_Callbacks.pfnInternalAllocation = nullptr;
	m_Callbacks.pfnInternalFree = nullptr;
	m_Callbacks.pfnAllocation = nullptr;
	m_Callbacks.pfnReallocation = nullptr;
	m_Callbacks.pfnFree = nullptr;
	m_Callbacks.pfnInternalAllocation = nullptr;
	m_Callbacks.pfnInternalFree = nullptr;


}
