#pragma once
#include <fstream>
#include <iostream>
#include "MeshObject.h"
#include "tinyGLTF/tiny_gltf.h"

// GLTFƒtƒ@ƒCƒ‹‚ğ“Ç‚İ‚Ş
static std::vector<MeshObject> LoadGLTF(const std::string& filePath);

