#pragma once
#include "Singleton.h"
#include "MeshObjectBuffer.h"


class VMeshManager
	final : public Singleton<VMeshManager>	//シングルトンパターンにする
{
private:
	friend class Singleton<VMeshManager>;
	VMeshManager();	// コンストラクタ


public:
	bool Load(VmaAllocator* allocator, MeshObject* pMeshObject);
	VMeshObject* GetObject(MeshObject* pMeshObject);

private:	// プライベート変数
	std::unordered_map<MeshObject*, VMeshObject> m_ObjectList;

};

