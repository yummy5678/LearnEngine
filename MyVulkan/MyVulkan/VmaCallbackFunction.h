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
	// アライメントに対応するメモリ確保
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
	// メモリの再割り当て時に呼び出される関数

	if (size == 0) {
		std::free(pOriginal);
		return nullptr;
	}

	// アライメントに対応する再確保（厳密には realloc はアライメント保証なし）
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











// GPU側のメモリの割り当てと解放時に呼び出される関数
static void AllocateCallbackFunction(VmaAllocator allocator, uint32_t memoryType, VkDeviceMemory memory, VkDeviceSize size, void* pUserData)
{
	// メモリの割り当てが行われたときに呼び出される関数
	// ここでメモリの割り当て情報を出力するなどの処理を行う
	std::cout << "Memory allocated: Type = " << memoryType
		<< ", Size = " << size << " bytes"
		<< ", Handle = " << memory << std::endl;
}

static void FreeCallbackFunction(VmaAllocator allocator, uint32_t memoryType, VkDeviceMemory memory, VkDeviceSize size, void* pUserData)
{
	// メモリの解放が行われたときに呼び出される関数
	// ここでメモリの解放情報を出力するなどの処理を行う
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







