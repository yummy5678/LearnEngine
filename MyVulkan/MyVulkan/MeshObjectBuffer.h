#pragma once
#include "MeshBuffer.h"
#include "ImageBuffer.h"


class VMeshObject
{
public:
	VMeshObject();
	~VMeshObject();

	void SetMeshObject(VmaAllocator allocator, MeshObject &meshObject);

	std::vector<VMesh>			GetMeshes();
	std::vector<VImageBuffer>	GetTexture();

private:
	std::vector<VMesh>			m_Mesh;
	std::vector<VImageBuffer>	m_Texture;	// 今はテクスチャ画像だけ。あとでマテリアルクラスに書き換える

	void SetMesh(VmaAllocator allocator, MeshObject& meshObject);
	void SetMaterial(VmaAllocator allocator, MeshObject& meshObject);
};

