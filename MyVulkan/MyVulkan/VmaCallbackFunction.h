#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include <iostream>


static void* AllocationFunction(
	void* pUserData,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	// �������̊��蓖�Ď��ɌĂяo�����֐�
	// ���̂Ƃ��뉽�����邱�Ƃ͂Ȃ��̂� nullptr ��Ԃ�
	return nullptr;
}

static void* ReallocationFunction(
	void* pUserData,
	void* pOriginal,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	// �������̍Ċ��蓖�Ď��ɌĂяo�����֐�
	// ���̂Ƃ��뉽�����邱�Ƃ͂Ȃ��̂� nullptr ��Ԃ�
	return nullptr;
}

static void FreeFunction(
	void* pUserData,
	void* pMemory) 
{
	std::cout << "Freeing memory: " << pMemory << std::endl;
	std::free(pMemory);
}

static void InternalAllocationFunction(
	void* pUserData,
	size_t size,
	VkInternalAllocationType allocationType,
	VkSystemAllocationScope allocationScope) {
	std::cout << "Internal allocation of size " << size
		<< ", type: " << allocationType
		<< ", scope: " << allocationScope << std::endl;
}

static void InternalFreeFunction(
	void* pUserData,
	size_t size,
	VkInternalAllocationType allocationType,
	VkSystemAllocationScope allocationScope) {
	std::cout << "Internal free of size " << size
		<< ", type: " << allocationType
		<< ", scope: " << allocationScope << std::endl;
}


static VkAllocationCallbacks AllocationCallbacks 
{
	nullptr, // pUserData;
	nullptr, //AllocationFunction,			// pfnAllocation;
	nullptr, // ReallocationFunction,		// pfnReallocation;
	nullptr, // FreeFunction,				// pfnFree;
	InternalAllocationFunction,	// pfnInternalAllocation;
	InternalFreeFunction		// pfnInternalFree;
};











// GPU���̃������̊��蓖�ĂƉ�����ɌĂяo�����֐�
static void AllocateCallbackFunction(VmaAllocator allocator, uint32_t memoryType, VkDeviceMemory memory, VkDeviceSize size, void* pUserData)
{
	// �������̊��蓖�Ă��s��ꂽ�Ƃ��ɌĂяo�����֐�
	// �����Ń������̊��蓖�ď����o�͂���Ȃǂ̏������s��
	std::cout << "Memory allocated: Type = " << memoryType
		<< ", Size = " << size << " bytes"
		<< ", Handle = " << memory << std::endl;
}

static void FreeCallbackFunction(VmaAllocator allocator, uint32_t memoryType, VkDeviceMemory memory, VkDeviceSize size, void* pUserData)
{
	// �������̉�����s��ꂽ�Ƃ��ɌĂяo�����֐�
	// �����Ń������̉�������o�͂���Ȃǂ̏������s��
	std::cout << "Memory freed: Type = " << memoryType
		<< ", Size = " << size << " bytes"
		<< ", Handle = " << memory << std::endl;
}


static VmaDeviceMemoryCallbacks deviceMemoryCallbacks
{
	AllocateCallbackFunction,
	FreeCallbackFunction,
	nullptr
};







