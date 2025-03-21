#pragma once
#include <fstream>
#include <iostream>
#include "MeshObject.h"

#define TINYGLTF_NO_STB_IMAGE       // stb_image�̎��������O����
#define TINYGLTF_NO_STB_IMAGE_WRITE // stb_image_write�̎��������O����
//#define TINYGLTF_USE_CPP14

#define STB_IMAGE_IMPLEMENTATION   // stb_image�̎�����L��������
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION // stb_image_write�̎�����L��������
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


