#pragma once
#include "MeshObject.h"
#include "MeshObjectBuffer.h"

#include "tinyGLTF/tiny_gltf.h"

// GLTF�t�@�C����ǂݍ���
static bool LoadGLTF(const std::string& filePath, MeshObject& meshObject);
