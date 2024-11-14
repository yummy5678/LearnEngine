#pragma once
#include "MeshObject.h"
#include "MaterialBuffer.h"
#include "MeshBuffer.h"




class SceneObject
{
public:
	SceneObject() = default;
	~SceneObject();

	void					SetMesh(VmaAllocator allocator, MeshObject& mesh);
	std::vector<VMesh>		GetMeshes();
	std::vector<VMaterial>	GetMaterials();

	void		SetTransform(glm::mat4 transform);
	Transform	GetTransform();



private:
	Transform				m_Transform;	// トランスフォーム(オブジェクトの座標とかの情報)


	std::vector<VMaterial>	m_Material;		// テクスチャ等のバッファが入っている
	std::vector<VMesh>		m_Mesh;			// 頂点情報等のバッファが入っている




};

