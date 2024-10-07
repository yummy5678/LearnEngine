#pragma once
#include "MeshObject.h"
#include "BufferGenerator.h"
#include "ImagesGenerator.h"

using Transform = glm::mat4;


class SceneObject
{
public:
	SceneObject();
	~SceneObject();

	void		SetPMesh(MeshObject* pMesh);
	MeshObject	GetMesh();

	void		SetTransform(glm::mat4 transform);
	glm::mat4	GetTransform();


	vk::Buffer	GetMeshBuffer();


private:
	MeshObject		m_Mesh;			// メッシュのポインタ(実態はメッシュマネージャーが管理)
	Transform		m_Transform;		// トランスフォーム(オブジェクトの座標とかの情報)

	BufferGenerator m_VertexBuffer;		// 頂点バッファ
	//ImagesGenerator m_TextureBuffer;	// テクスチャーバッファ
};

