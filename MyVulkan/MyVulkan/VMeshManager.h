#pragma once
#include "MeshObjectBuffer.h"


class VMeshManager
{
	VMeshManager();	// コンストラクタ
	~VMeshManager();	// コンストラクタ


public:
	bool Load(VmaAllocator* allocator, MeshObject* pMeshObject);
	VMeshObject* GetObject(MeshObject* pMeshObject);

private:	// プライベート変数
	std::unordered_map<MeshObject*, VMeshObject> m_ObjectList;

};

