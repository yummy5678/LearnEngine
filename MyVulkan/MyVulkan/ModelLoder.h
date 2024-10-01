#pragma once
#include <assimp/Importer.hpp>  // Assimpのインポーターを使用
#include <assimp/scene.h>       // Assimpのシーン構造体を使用
#include <assimp/postprocess.h> // モデル処理のオプションを指定

#include <iostream>

#include "SceneObject.h"




class ModelLoder
{
public:
	ModelLoder();
	~ModelLoder();

	bool Load(std::string filePath);

	MeshObject GetModel();

	//void draw(VkCommandBuffer commandBuffer);
	void cleanup();



private:
	MeshObject m_Model;


};

