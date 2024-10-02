#pragma once
#include "MeshObject.h"
#include "BufferGenerator.h"
#include "ImagesGenerator.h"

class SceneObject
{
public:
	SceneObject();
	~SceneObject();

	void		SetMesh(MeshObject* pMesh);
	MeshObject* GetPMesh();

	void		SetTransform(glm::mat4 transform);
	glm::mat4	GetTransform();


	vk::Buffer	GetMeshBuffer();


private:
	MeshObject*		m_pMesh;			// メッシュのポインタ(実態はメッシュマネージャーが管理)
	glm::mat4		m_Transform;		// トランスフォーム(オブジェクトの座標とかの情報)

	BufferGenerator m_VertexBuffer;		// 頂点バッファ
	//ImagesGenerator m_TextureBuffer;	// テクスチャーバッファ
};

