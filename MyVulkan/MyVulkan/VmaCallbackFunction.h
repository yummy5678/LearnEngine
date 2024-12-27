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
	// �A���C�����g�ɑΉ����郁�����m��
	void* ptr = _aligned_malloc(size, alignment);
	if (ptr == nullptr) {
		std::cerr << "Allocation failed. Size: " << size << ", Alignment: " << alignment << std::endl;
	}
	return ptr;
}

static void* ReallocationFunction(
	void* pUserData,
	void* pOriginal,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope allocationScope)
{
	// �������̍Ċ��蓖�Ď��ɌĂяo�����֐�

	if (size == 0) {
		std::free(pOriginal);
		return nullptr;
	}

	// �A���C�����g�ɑΉ�����Ċm�ہi�����ɂ� realloc �̓A���C�����g�ۏ؂Ȃ��j
	void* ptr = std::realloc(pOriginal, size);
	if (ptr == nullptr) {
		std::cerr << "Reallocation failed. Size: " << size << ", Alignment: " << alignment << std::endl;
	}
	return ptr;
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
	AllocationFunction,			// pfnAllocation;
	ReallocationFunction,		// pfnReallocation;
	FreeFunction,				// pfnFree;
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







