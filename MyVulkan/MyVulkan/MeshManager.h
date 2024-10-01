#pragma once
#include <iostream>
#include <assimp/Importer.hpp>  // Assimpのインポーターを使用
#include <assimp/scene.h>       // Assimpのシーン構造体を使用
#include <assimp/postprocess.h> // モデル処理のオプションを指定

#include "Singleton.h"
#include "MeshObject.h"



class MeshManager 
	final : public Singleton<MeshManager>	//シングルトンパターンにする
{
private:
	friend class Singleton<MeshManager>;
	MeshManager();	// コンストラクタ

public:
	bool Load(std::string filePath);
	MeshObject GetMesh(std::string filePath);




private:	// プライベート変数
	std::unordered_map<std::string, MeshObject> m_MeshList;






private:	// プライベート関数
	// ノードを処理するプライベート関数
	MeshObject	ProcessNode(aiNode* node, const aiScene* scene);
	// メッシュを処理するプライベート関数
	Mesh		ProcessMesh(aiMesh* mesh, const aiScene* scene);
	Material	ProcessMaterial(aiMaterial* material);
};

