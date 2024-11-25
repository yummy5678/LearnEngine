#pragma once
#include "MeshObject.h"
#include "MaterialBuffer.h"
#include "MeshBuffer.h"
#include "MeshObjectBuffer.h"
#include <memory> // std::shared_ptrを使用

class SceneObject
{
public:
	SceneObject() = default;
	~SceneObject();

	void					SetMesh(VmaAllocator* allocator, VMeshObject* mesh);
	std::shared_ptr<std::vector<VMesh>>		GetMeshes();
	std::shared_ptr<std::vector<VMaterial>>	GetMaterials();

	void		SetTransform(glm::mat4 transform);
	Transform* GetPTransform();

private:
	Transform				m_Transform;	// トランスフォーム(オブジェクトの座標とかの情報)
	std::shared_ptr<std::vector<VMaterial>>	m_Material;		// テクスチャ等のバッファが入っている
	std::shared_ptr<std::vector<VMesh>>		m_Mesh;			// 頂点情報等のバッファが入っている
};
