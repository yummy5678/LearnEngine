#pragma once
#include <vulkan/vulkan.hpp>
#include <VMA/vk_mem_alloc.h>
#include "MeshObject.h"
#include "StagingBuffer.h"

// RAMに置かれている画像データをVRAMに移行するクラス
class VImageBuffer
{
public:
	VImageBuffer();
	~VImageBuffer();


	void SetImage(VmaAllocator allocator, Texture& texture, vk::Format format);

	vk::Image GetImage();

protected:
	// 結びつける先のデバイス
	VmaAllocator			m_Allocator;

	// バッファのタイプ
	vk::BufferUsageFlags	m_Usage;

	// キュー間の読み取り設定
	vk::SharingMode			m_SharingMode = vk::SharingMode::eExclusive;

	vk::Image				m_Buffer;


};

