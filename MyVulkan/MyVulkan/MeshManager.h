#pragma once
#include <iostream>
#include <assimp/Importer.hpp>  // Assimpのインポーターを使用
#include <assimp/scene.h>       // Assimpのシーン構造体を使用
#include <assimp/postprocess.h> // モデル処理のオプションを指定

// STB
#define STB_IMAGE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "MeshObject.h"
#include "MeshObjectBuffer.h"


class MeshLoder 
{
public:
	MeshLoder();	// コンストラクタ
	~MeshLoder();	

	std::shared_ptr<VMeshObject> Load(VmaAllocator* pAllocator, std::string filePath);


private:	// プライベート関数
	// ノードを処理するプライベート関数
	MeshObject	ProcessNode(aiNode* node, const aiScene* scene);
	// メッシュを処理するプライベート関数
	Mesh		ProcessMesh(aiMesh* mesh, const aiScene* scene);
	Material	ProcessMaterial(aiMaterial* material);

	// テクスチャの読み込み関数
	Texture		LoadTextureFile(std::string fileName);
};

