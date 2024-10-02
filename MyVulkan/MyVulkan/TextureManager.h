#pragma once
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Singleton.h"


class TextureManager
	final : public Singleton<TextureManager>	//シングルトンパターンにする
{
private:
	friend class Singleton<TextureManager>;
	TextureManager();	// コンストラクタ

public:
	bool Load(std::string filePath);
	int* GetTexture(std::string filePath);




private:	// プライベート変数
	std::unordered_map<std::string, int> m_MeshList;




private:








};

