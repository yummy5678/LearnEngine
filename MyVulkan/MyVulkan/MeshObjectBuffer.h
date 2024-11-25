#pragma once
#include "MeshBuffer.h"
#include "ImageBuffer.h"
#include "MeshObject.h" 
#include <memory> 

class VMeshObject
{
public:
	VMeshObject();
	~VMeshObject();

	void SetMeshObject(VmaAllocator* allocator, MeshObject& meshObject);

	std::shared_ptr<std::vector<VMesh>>				GetMeshes();
	std::shared_ptr<std::vector<VTextureBuffer>>	GetTexture();

private:
	std::shared_ptr<std::vector<VMesh>>				m_Mesh;
	std::shared_ptr<std::vector<VTextureBuffer>>	m_Texture;	// 今はテクスチャ画像だけ。あとでマテリアルクラスに書き換える

	void SetMesh(VmaAllocator* allocator, MeshObject& meshObject);
	void SetMaterial(VmaAllocator* allocator, MeshObject& meshObject);
};
