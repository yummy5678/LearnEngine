#pragma once
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Singleton.h"


class TextureManager
	final : public Singleton<TextureManager>	//�V���O���g���p�^�[���ɂ���
{
private:
	friend class Singleton<TextureManager>;
	TextureManager();	// �R���X�g���N�^

public:
	bool Load(std::string filePath);
	int* GetTexture(std::string filePath);




private:	// �v���C�x�[�g�ϐ�
	std::unordered_map<std::string, int> m_MeshList;




private:








};

