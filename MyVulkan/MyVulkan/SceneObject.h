#pragma once
#include "MeshObject.h"
#include "BufferGenerator.h"
#include "ImagesGenerator.h"
#include "ImageBuffer.h"

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


	vk::Buffer	GetVertexBuffer();


private:
	MeshObject		m_Mesh;			// メッシュのポインタ(実態はメッシュマネージャーが管理)
	Transform		m_Transform;	// トランスフォーム(オブジェクトの座標とかの情報)


	VVertexBuffer	m_VertexBuffer;	// 頂点バッファ
	VImageBuffer	m_Texture;		// テクスチャーバッファ


};

