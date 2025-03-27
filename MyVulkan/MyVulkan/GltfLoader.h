#pragma once
#include <fstream>
#include <iostream>
#include "MeshObject.h"
   
#include "stb/stb_image.h"

#include "stb/stb_image_write.h" 

#include "tinyGLTF/tiny_gltf.h"



class FileLoader
{
public:
    static std::vector<MeshObject> LoadGLTF(const std::string& filePath);

private:

    // GLTF�t�@�C����ǂݍ���
    // ���b�V���̏���
    static void ProcessMeshes(const tinygltf::Model& model, MeshObject& meshObject);

    // �}�e���A���̏���
    static void ProcessMaterials(const tinygltf::Model& model, MeshObject& meshObject);



};


