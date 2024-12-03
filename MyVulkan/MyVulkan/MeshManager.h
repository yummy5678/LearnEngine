#pragma once
#include <iostream>
#include <assimp/Importer.hpp>  // Assimp�̃C���|�[�^�[���g�p
#include <assimp/scene.h>       // Assimp�̃V�[���\���̂��g�p
#include <assimp/postprocess.h> // ���f�������̃I�v�V�������w��

// STB
#define STB_IMAGE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "MeshObject.h"
#include "MeshObjectBuffer.h"


class MeshLoder 
{
public:
	MeshLoder();	// �R���X�g���N�^
	~MeshLoder();	

	std::shared_ptr<VMeshObject> Load(VmaAllocator* pAllocator, std::string filePath);


private:	// �v���C�x�[�g�֐�
	// �m�[�h����������v���C�x�[�g�֐�
	MeshObject	ProcessNode(aiNode* node, const aiScene* scene);
	// ���b�V������������v���C�x�[�g�֐�
	Mesh		ProcessMesh(aiMesh* mesh, const aiScene* scene);
	Material	ProcessMaterial(aiMaterial* material);

	// �e�N�X�`���̓ǂݍ��݊֐�
	Texture		LoadTextureFile(std::string fileName);
};

