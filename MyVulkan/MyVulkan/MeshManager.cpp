#include "MeshManager.h"


MeshLoder::MeshLoder()
{
}

MeshLoder::~MeshLoder()
{
}

bool MeshLoder::Load(VmaAllocator* pAllocator, std::string filePath)
{
    // Assimp�̃C���|�[�^�[���쐬
    Assimp::Importer importer;  

    // ���f���t�@�C����ǂݍ��݁A�K�v�Ȍ㏈���I�v�V�������w��
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    // �ǂݍ��݂����s�����ꍇ��A�V�[�����s���S�ȏꍇ�ɃG���[�`�F�b�N
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error: " << importer.GetErrorString() << std::endl;
        return false; // ���f���̓ǂݍ��݂Ɏ��s������false��Ԃ�
    }

    MeshObject mesh = ProcessNode(scene->mRootNode, scene);

    VMeshObject object;
	object.SetMeshObject(pAllocator, mesh);

    // �V�[���̃��[�g�m�[�h����ċA�I�Ƀm�[�h���������Ă���
    m_MeshList[filePath] = std::make_shared<VMeshObject>(object);

    // �ǂݍ��݂ɐ���
    return true; 
}

std::shared_ptr<VMeshObject> MeshLoder::GetVMesh(std::string filePath)
{
    auto it = m_MeshList.find(filePath);
    if (it != m_MeshList.end()) {
        return std::make_shared<VMeshObject>(it->second);
    }
    return nullptr;
}

// �V�[���̃m�[�h���ċA�I�ɏ������郁�\�b�h
MeshObject MeshLoder::ProcessNode(aiNode* node, const aiScene* scene)
{
    MeshObject object;
    // �m�[�h�������b�V�������ׂď���
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; // �m�[�h���̃��b�V�����擾
        object.meshes.push_back(ProcessMesh(mesh, scene)); // ���b�V�����������A���ʂ�ۑ�
    }

    // �q�m�[�h������ꍇ�A�ċA�I�ɏ�������i�V�[���S�̂�T���j
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene); // �q�m�[�h���ċA�I�ɏ���
    }

    return object;
}

// ���b�V�����������A���_��C���f�b�N�X�f�[�^�𒊏o���郁�\�b�h
Mesh MeshLoder::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    Mesh resultMesh; // ���b�V�����i�[���邽�߂̕ϐ�
    std::vector<Material> resultMaterials;

    // ���b�V���̊e���_������
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex; // ���_�̏����i�[����ϐ�

        // ���_�̈ʒu���擾���AVertex�\���̂ɕۑ�
        vertex.position = {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };

        // �@���x�N�g�������݂���ꍇ�A�擾���ĕۑ�
        if (mesh->mNormals) {
            vertex.normal = {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            };
        }

        // �e�N�X�`�����W�����݂���ꍇ�A�擾���ĕۑ��i�����̃e�N�X�`�����W�Z�b�g�����݂���ꍇ�����邪�A�����ł�1�Z�b�g�ڂ̂݁j
        if (mesh->mTextureCoords[0]) {
            vertex.textureCoord = {
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            };
        }
        else {
            vertex.textureCoord = { 0.0f, 0.0f }; // �e�N�X�`�����W���Ȃ��ꍇ�A�f�t�H���g�l��ݒ�
        }

        // �����������_�����b�V���ɒǉ�
        resultMesh.vertices.push_back(vertex);
    }

    // ���b�V���̊e�t�F�C�X�i�ʁj���������A�C���f�b�N�X�f�[�^�𒊏o
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i]; // �t�F�C�X���擾
        // �t�F�C�X�̃C���f�b�N�X�����b�V���ɒǉ�
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            resultMesh.indices.push_back(face.mIndices[j]);
        }
    }

    // ���b�V���Ɋ֘A����}�e���A��������ꍇ�A�}�e���A��������
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; // �}�e���A�����擾
        resultMaterials.push_back(ProcessMaterial(material)); // �}�e���A�����������A���ʂ�ۑ�        
    }

    return resultMesh; // �����������b�V����Ԃ�
}

// �}�e���A�����������A�F��e�N�X�`���̏��𒊏o���郁�\�b�h
Material MeshLoder::ProcessMaterial(aiMaterial* material) {
    Material resultMaterial; // �}�e���A�������i�[���邽�߂̕ϐ�

    // �}�e���A���̐F���擾�i���݂��Ȃ��ꍇ�A�f�t�H���g�l���g�p�j
    aiColor4D diffuseColor;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor)) {
        resultMaterial.baseColor = glm::vec4(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);
    }
    else {
        resultMaterial.baseColor = glm::vec4(1.0f); // �f�t�H���g�͔��F
    }

    // �}�e���A���̑e������������擾�i���݂��Ȃ��ꍇ�A�f�t�H���g�l���g�p�j
    float roughness = 0.0f, metallic = 0.0f;
    material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness); // �e�����擾
    material->Get(AI_MATKEY_METALLIC_FACTOR, metallic);   // ���������擾
    resultMaterial.roughness = roughness;
    resultMaterial.metallic = metallic;

    // �}�e���A���Ɋ֘A����e�N�X�`���p�X���擾(�e�N�X�`�������݂���ꍇ�̂�)
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString textureName;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName);
        resultMaterial.texture = LoadTextureFile(textureName.C_Str()); // �e�N�X�`���̃p�X��ۑ�
    }

    return resultMaterial; // ���������}�e���A����Ԃ�
}

Texture MeshLoder::LoadTextureFile(std::string fileName)
{
    Texture result;
    int width, height, channel;
    result.data = (uint8_t)stbi_load(fileName.c_str(), &width, &height, &channel, STBI_rgb_alpha);
    result.width = width;
    result.height = height;
    result.channel = channel;

    if (!result.data)
    {
        throw std::runtime_error("�摜�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B: " + fileName);
    }

    return result;
}


