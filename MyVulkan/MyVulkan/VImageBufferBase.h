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
		vk::ImageUsageFlags		bufferusage,		// 画像の使用用途 
		vk::SharingMode			sharingMode,		// 画像のキュー間での共有設定
		vk::ImageAspectFlags	aspectFlag,			// 画像のアスペクトフラグ
		vk::Format				format,				// 画像のフォーマット
		vk::MemoryPropertyFlags requiredFlag,		// 使用するバッファの必須要件
		vk::MemoryPropertyFlags preferredFlag,		// 使用するバッファの優先要件
		VmaAllocationCreateFlags allocationFlag);	// メモリの割り当て方式
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

	// 作成時に登録したアロケーターを返す
	VmaAllocator* GetUsingAllocator();


protected:
	VmaAllocator* m_pAllocator;


	// バッファのタイプ
	vk::ImageUsageFlags		m_Usage;
	// キュー間の読み取り設定
	vk::SharingMode			m_SharingMode;
	vk::Format				m_Format;
	vk::ImageAspectFlags	m_AspectFlag;
	vk::Extent3D			m_Extent;

	ImageSet m_ImageSet;	// イメージとイメージ情報のセット

	VmaAllocation			m_ImageAllocation;	// データバッファの領域	

	VmaAllocationCreateFlags	m_AllocationFlag;	// メモリの割り当て方式
	vk::MemoryPropertyFlags		m_RequiredFlag;		// メモリの必須条件
	vk::MemoryPropertyFlags		m_PreferredFlag;	// メモリの優先条件

	VkImageCreateInfo CreateImageInfo(uint32_t imageWidth, uint32_t imageHeight);
	VkImageCreateInfo CreateImageInfo(vk::Extent3D extent);
	void CreateBuffer(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight);
	void CreateBuffer(VmaAllocator* allocator, vk::Extent2D extent);
	void CreateImageView(vk::Device logicalDevice, vk::Image imageBuffer, vk::Format format, vk::ImageAspectFlags aspectFlag);


	// データをバッファに書き込む
	/*void MapData(void* setData, vk::DeviceSize dataSize);*/
};

