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
	void* ptr = _aligned_malloc(size, alignment); // �A���C�����g�Ή��m��
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
	if (size == 0) {
		_aligned_free(pOriginal);
		return nullptr;
	}

	void* ptr = _aligned_malloc(size, alignment); // �V�K�m��
	if (ptr && pOriginal) {
		std::memcpy(ptr, pOriginal, size); // �K�v�ɉ����ăR�s�[�i���S�̂��߃R�s�[�ʐ�����ǉ����Ă��悢�j
		_aligned_free(pOriginal);
	}

	if (ptr == nullptr) {
		std::cerr << "Reallocation failed. Size: " << size << ", Alignment: " << alignment << std::endl;
	}
	return ptr;
}

static void FreeFunction(
	void* pUserData,
	void* pMemory) 
{
	std::cout << "����������� : " << pMemory << std::endl;
	_aligned_free(pMemory);
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







