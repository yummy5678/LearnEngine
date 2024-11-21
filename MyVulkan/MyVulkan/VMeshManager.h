#pragma once
#include "MeshObjectBuffer.h"


class VMeshManager
{
	VMeshManager();	// �R���X�g���N�^
	~VMeshManager();	// �R���X�g���N�^


public:
	bool Load(VmaAllocator* allocator, MeshObject* pMeshObject);
	VMeshObject* GetObject(MeshObject* pMeshObject);

private:	// �v���C�x�[�g�ϐ�
	std::unordered_map<MeshObject*, VMeshObject> m_ObjectList;

};

