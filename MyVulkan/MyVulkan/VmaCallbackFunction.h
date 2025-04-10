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
	void* ptr = _aligned_malloc(size, alignment); // アライメント対応確保
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

	void* ptr = _aligned_malloc(size, alignment); // 新規確保
	if (ptr && pOriginal) {
		std::memcpy(ptr, pOriginal, size); // 必要に応じてコピー（安全のためコピー量制限を追加してもよい）
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
	std::cout << "メモリを解放 : " << pMemory << std::endl;
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







