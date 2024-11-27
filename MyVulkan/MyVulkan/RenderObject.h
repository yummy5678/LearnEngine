#pragma once
#include "MeshObject.h"
#include "MaterialBuffer.h"
#include "MeshBuffer.h"
#include "MeshObjectBuffer.h"
#include <memory> // std::shared_ptrを使用


// オブジェクトの描画時に必要な情報をまとめたクラス
class RenderObject
{
public:
	RenderObject();
	~RenderObject();

	void SetMesh(VmaAllocator* allocator, std::shared_ptr<VMeshObject> mesh);
	std::vector<VMesh>&				GetMeshes();
	std::vector<VTextureBuffer>&	GetMaterials();

	void		SetTransform(glm::mat4 transform);
	Transform* GetPTransform();

private:
	Transform						m_Transform;	// トランスフォーム(オブジェクトの座標とかの情報)
	std::shared_ptr<VMeshObject>	m_Mesh; 

};
