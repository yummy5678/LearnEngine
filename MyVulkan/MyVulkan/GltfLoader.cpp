#include "GltfLoader.h"



// ���b�V���̏���
void FileLoader::ProcessMeshes(const tinygltf::Model& model, MeshObject& meshObject)
{
    for (const auto& gltfMesh : model.meshes) {
        for (const auto& primitive : gltfMesh.primitives) {
            Mesh mesh;

            // ���_���W�̎擾
            auto it = primitive.attributes.find("POSITION");
            if (it != primitive.attributes.end()) {
                const tinygltf::Accessor& accessor = model.accessors[it->second];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                const float* positions = reinterpret_cast<const float*>(
                    buffer.data.data() + bufferView.byteOffset + accessor.byteOffset);

                for (size_t i = 0; i < accessor.count; ++i) {
                    Vertex vertex;
                    vertex.position = glm::vec3(
                        positions[i * 3 + 0],
                        positions[i * 3 + 1],
                        positions[i * 3 + 2]);
                    mesh.vertices.push_back(vertex);
                }
            }

            // �@���x�N�g���̎擾
            it = primitive.attributes.find("NORMAL");
            if (it != primitive.attributes.end()) {
                const tinygltf::Accessor& accessor = model.accessors[it->second];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                const float* normals = reinterpret_cast<const float*>(
                    buffer.data.data() + bufferView.byteOffset + accessor.byteOffset);

                for (size_t i = 0; i < accessor.count; ++i) {
                    mesh.vertices[i].normal = glm::vec3(
                        normals[i * 3 + 0],
                        normals[i * 3 + 1],
                        normals[i * 3 + 2]);
                }
            }

            // �e�N�X�`�����W�̎擾
            it = primitive.attributes.find("TEXCOORD_0");
            if (it != primitive.attributes.end()) {
                const tinygltf::Accessor& accessor = model.accessors[it->second];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                const float* texCoords = reinterpret_cast<const float*>(
                    buffer.data.data() + bufferView.byteOffset + accessor.byteOffset);

                for (size_t i = 0; i < accessor.count; ++i) {
                    mesh.vertices[i].textureCoord = glm::vec2(
                        texCoords[i * 2 + 0],
                        texCoords[i * 2 + 1]);
                }
            }

            // �C���f�b�N�X�f�[�^�̎擾
            if (primitive.indices > -1) {
                const tinygltf::Accessor& accessor = model.accessors[primitive.indices];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
                const uint16_t* indices = reinterpret_cast<const uint16_t*>(
                    buffer.data.data() + bufferView.byteOffset + accessor.byteOffset);

                for (size_t i = 0; i < accessor.count; ++i) {
                    mesh.indices.push_back(indices[i]);
                }
            }

            meshObject.mesh = mesh;
        }
    }
}

// �}�e���A���̏���
void FileLoader::ProcessMaterials(const tinygltf::Model& model, MeshObject& meshObject) {
    for (const auto& gltfMaterial : model.materials) {
        Material material;

        // ��{�F
        auto it = gltfMaterial.values.find("baseColorFactor");
        if (it != gltfMaterial.values.end()) {
            const auto& color = it->second.ColorFactor();
            material.baseColor = glm::vec4(color[0], color[1], color[2], color[3]);
        }

        // ������
        auto metallicIt = gltfMaterial.values.find("metallicFactor");
        if (metallicIt != gltfMaterial.values.end()) {
            material.metallic = metallicIt->second.Factor();
        }

        // �e��
        auto roughnessIt = gltfMaterial.values.find("roughnessFactor");
        if (roughnessIt != gltfMaterial.values.end()) {
            material.roughness = roughnessIt->second.Factor();
        }

        // �e�N�X�`���f�[�^
        auto texIt = gltfMaterial.values.find("baseColorTexture");
        if (texIt != gltfMaterial.values.end()) {
            int textureIndex = texIt->second.TextureIndex();
            const tinygltf::Texture& gltfTexture = model.textures[textureIndex];
            const tinygltf::Image& image = model.images[gltfTexture.source];

            material.texture.pixelData = image.image;
            material.texture.width = image.width;
            material.texture.height = image.height;
            material.texture.channel = static_cast<TextureChannel>(image.component);
        }

        meshObject.material = material;
    }
}

std::vector<MeshObject> FileLoader::LoadGLTF(const std::string& filePath)
{

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    // �t�@�C���ǂݍ���
    bool success = loader.LoadASCIIFromFile(&model, &err, &warn, filePath);
    if (!warn.empty()) 
    {
        std::cout << "�x��: " << warn << std::endl;
    }
    if (!err.empty()) 
    {
        std::cerr << "�G���[: " << err << std::endl;
        return std::vector<MeshObject>();
    }
    if (!success) 
    {
        std::cerr << "���f���̓ǂݍ��݂Ɏ��s���܂����B" << std::endl;
        return std::vector<MeshObject>();
    }

    // ���f���̏���\�������
    std::cout << "Number of scenes: " << model.scenes.size() << std::endl;
    std::cout << "Number of nodes: "  << model.nodes.size() << std::endl;
    std::cout << "Number of meshes: " << model.meshes.size() << std::endl;

    std::vector<MeshObject> meshObjects;

    meshObjects.resize(model.nodes.size());

    for (uint32_t i = 0; i < model.nodes.size(); i++)
    {
        auto node = model.nodes[i];

        // �m�[�h�̖��O�����擾
        meshObjects[i].name.assign(node.name);

        // �g�����X�t�H�[�������擾
        if (!node.matrix.empty())
        meshObjects[i].transform = glm::make_mat4(node.matrix.data());

        // ���b�V���ƃ}�e���A�����擾����
        ProcessMeshes(model, meshObjects[i]);
        ProcessMaterials(model, meshObjects[i]);
    }


    return meshObjects;
}

