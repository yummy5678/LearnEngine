#pragma once
#include "VImageBufferBase.h"
#include "MeshObject.h"
#include "StagingImageBuffer.h"

// RAMに置かれている画像データをVRAMに移行するクラス
class VMeshTextureBuffer : public VImageBufferBase
{
public:
	VMeshTextureBuffer();
	~VMeshTextureBuffer();
	void Cleanup();

	void SetImage(VmaAllocator* allocator, Texture& texture, vk::Fence fence);

protected:
	vk::Extent2D m_Extent;

};

