#pragma once
#include "Singleton.h"
#include "MeshObjectBuffer.h"


class VMeshManager
	final : public Singleton<VMeshManager>	//�V���O���g���p�^�[���ɂ���
{
private:
	friend class Singleton<VMeshManager>;
	VMeshManager();	// �R���X�g���N�^


public:
	bool Load(VmaAllocator* allocator, MeshObject* pMeshObject);
	VMeshObject* GetObject(MeshObject* pMeshObject);

private:	// �v���C�x�[�g�ϐ�
	std::unordered_map<MeshObject*, VMeshObject> m_ObjectList;

};

