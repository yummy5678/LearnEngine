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

    // GLTFファイルを読み込む
    // メッシュの処理
    static void ProcessMeshes(const tinygltf::Model& model, MeshObject& meshObject);

    // マテリアルの処理
    static void ProcessMaterials(const tinygltf::Model& model, MeshObject& meshObject);



};


