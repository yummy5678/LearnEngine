#pragma once
#include "VImageBufferBase.h"
#include "MeshObject.h"
#include "StagingImageBuffer.h"

// RAM�ɒu����Ă���摜�f�[�^��VRAM�Ɉڍs����N���X
class VTextureBuffer : public VImageBufferBase
{
public:
	VTextureBuffer();
	~VTextureBuffer();
	void Cleanup();

	void SetImage(VmaAllocator* allocator, Texture& texture);

	vk::Image GetImageBuffer();

	vk::ImageView GetImageView();

protected:
	vk::Extent2D m_Extent;

};

