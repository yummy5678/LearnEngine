#pragma once
#include <assimp/Importer.hpp>  // Assimp�̃C���|�[�^�[���g�p
#include <assimp/scene.h>       // Assimp�̃V�[���\���̂��g�p
#include <assimp/postprocess.h> // ���f�������̃I�v�V�������w��

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

