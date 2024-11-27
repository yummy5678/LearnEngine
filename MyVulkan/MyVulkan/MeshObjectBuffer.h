#pragma once
#include "MeshBuffer.h"
#include "ImageBuffer.h"
#include "MeshObject.h" 


class VMeshObject
{
public:
	VMeshObject();
	~VMeshObject();

	void SetMeshObject(VmaAllocator* allocator, MeshObject& meshObject);

	std::vector<VMesh>&				GetMeshes();
	std::vector<VTextureBuffer>&	GetTexture();

private:
	std::vector<VMesh>			m_Mesh;
	std::vector<VTextureBuffer>	m_Texture;	// ���̓e�N�X�`���摜�����B���ƂŃ}�e���A���N���X�ɏ���������

	void SetMesh(VmaAllocator* allocator, MeshObject& meshObject);
	void SetMaterial(VmaAllocator* allocator, MeshObject& meshObject);
};
