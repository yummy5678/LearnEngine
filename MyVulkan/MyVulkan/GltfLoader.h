#pragma once
#include <fstream>
#include <iostream>
#include "MeshObject.h"
#include "tinyGLTF/tiny_gltf.h"

// GLTF�t�@�C����ǂݍ���
static std::vector<MeshObject> LoadGLTF(const std::string& filePath);

