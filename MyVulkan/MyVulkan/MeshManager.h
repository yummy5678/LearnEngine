#pragma once
#include <iostream>
#include <assimp/Importer.hpp>  // Assimp�̃C���|�[�^�[���g�p
#include <assimp/scene.h>       // Assimp�̃V�[���\���̂��g�p
#include <assimp/postprocess.h> // ���f�������̃I�v�V�������w��

#include "Singleton.h"
#include "MeshObject.h"



class MeshManager 
	final : public Singleton<MeshManager>	//�V���O���g���p�^�[���ɂ���
{
private:
	friend class Singleton<MeshManager>;
	MeshManager();	// �R���X�g���N�^

public:
	bool Load(std::string filePath);
	MeshObject GetMesh(std::string filePath);




private:	// �v���C�x�[�g�ϐ�
	std::unordered_map<std::string, MeshObject> m_MeshList;






private:	// �v���C�x�[�g�֐�
	// �m�[�h����������v���C�x�[�g�֐�
	MeshObject	ProcessNode(aiNode* node, const aiScene* scene);
	// ���b�V������������v���C�x�[�g�֐�
	Mesh		ProcessMesh(aiMesh* mesh, const aiScene* scene);
	Material	ProcessMaterial(aiMaterial* material);
};

