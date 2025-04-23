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

	void SetMesh(VmaAllocator* allocator, std::vector<MeshObject>* mesh);
	std::vector<VMeshObject>* GetMeshes();

	void Cleanup();


	void		SetTransform(glm::mat4 transform);
	Transform*  GetPTransform();

private:
	Transform					m_Transform;	// トランスフォーム(オブジェクトの座標とかの情報)
	std::vector<VMeshObject>	m_Meshes;
	


};
