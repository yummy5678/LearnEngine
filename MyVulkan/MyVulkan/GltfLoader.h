#pragma once
#include <fstream>
#include <iostream>
#include "MeshObject.h"

#define TINYGLTF_NO_STB_IMAGE       // stb_imageの実装を除外する
#define TINYGLTF_NO_STB_IMAGE_WRITE // stb_image_writeの実装を除外する
//#define TINYGLTF_USE_CPP14

#define STB_IMAGE_IMPLEMENTATION   // stb_imageの実装を有効化する
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION // stb_image_writeの実装を有効化する
#include "stb/stb_image_write.h" 
#include "tinyGLTF/tiny_gltf.h"


class FileLoader
{
public:
    static std::vector<MeshObject> LoadGLTF(const std::string& filePath);

private:

    // GLTFファイルを読み込む
    // メッシュの処理
    static void ProcessMeshes(const tinygltf::Model& model, MeshObject& meshObject);

    // マテリアルの処理
    static void ProcessMaterials(const tinygltf::Model& model, MeshObject& meshObject);



};


