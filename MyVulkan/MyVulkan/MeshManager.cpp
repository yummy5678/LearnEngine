#include "MeshManager.h"


MeshLoder::MeshLoder()
{
}

MeshLoder::~MeshLoder()
{
}

bool MeshLoder::Load(VmaAllocator* pAllocator, std::string filePath)
{
    // Assimpのインポーターを作成
    Assimp::Importer importer;  

    // モデルファイルを読み込み、必要な後処理オプションを指定
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    // 読み込みが失敗した場合や、シーンが不完全な場合にエラーチェック
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error: " << importer.GetErrorString() << std::endl;
        return false; // モデルの読み込みに失敗したらfalseを返す
    }

    MeshObject mesh = ProcessNode(scene->mRootNode, scene);

    VMeshObject object;
	object.SetMeshObject(pAllocator, mesh);

    // シーンのルートノードから再帰的にノードを処理していく
    m_MeshList[filePath] = std::make_shared<VMeshObject>(object);

    // 読み込みに成功
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

// シーンのノードを再帰的に処理するメソッド
MeshObject MeshLoder::ProcessNode(aiNode* node, const aiScene* scene)
{
    MeshObject object;
    // ノードが持つメッシュをすべて処理
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; // ノード内のメッシュを取得
        object.meshes.push_back(ProcessMesh(mesh, scene)); // メッシュを処理し、結果を保存
    }

    // 子ノードがある場合、再帰的に処理する（シーン全体を探索）
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene); // 子ノードを再帰的に処理
    }

    return object;
}

// メッシュを処理し、頂点やインデックスデータを抽出するメソッド
Mesh MeshLoder::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    Mesh resultMesh; // メッシュを格納するための変数
    std::vector<Material> resultMaterials;

    // メッシュの各頂点を処理
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex; // 頂点の情報を格納する変数

        // 頂点の位置を取得し、Vertex構造体に保存
        vertex.position = {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };

        // 法線ベクトルが存在する場合、取得して保存
        if (mesh->mNormals) {
            vertex.normal = {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            };
        }

        // テクスチャ座標が存在する場合、取得して保存（複数のテクスチャ座標セットが存在する場合もあるが、ここでは1セット目のみ）
        if (mesh->mTextureCoords[0]) {
            vertex.textureCoord = {
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            };
        }
        else {
            vertex.textureCoord = { 0.0f, 0.0f }; // テクスチャ座標がない場合、デフォルト値を設定
        }

        // 処理した頂点をメッシュに追加
        resultMesh.vertices.push_back(vertex);
    }

    // メッシュの各フェイス（面）を処理し、インデックスデータを抽出
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i]; // フェイスを取得
        // フェイスのインデックスをメッシュに追加
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            resultMesh.indices.push_back(face.mIndices[j]);
        }
    }

    // メッシュに関連するマテリアルがある場合、マテリアルを処理
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; // マテリアルを取得
        resultMaterials.push_back(ProcessMaterial(material)); // マテリアルを処理し、結果を保存        
    }

    return resultMesh; // 処理したメッシュを返す
}

// マテリアルを処理し、色やテクスチャの情報を抽出するメソッド
Material MeshLoder::ProcessMaterial(aiMaterial* material) {
    Material resultMaterial; // マテリアル情報を格納するための変数

    // マテリアルの色を取得（存在しない場合、デフォルト値を使用）
    aiColor4D diffuseColor;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor)) {
        resultMaterial.baseColor = glm::vec4(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);
    }
    else {
        resultMaterial.baseColor = glm::vec4(1.0f); // デフォルトは白色
    }

    // マテリアルの粗さや金属感を取得（存在しない場合、デフォルト値を使用）
    float roughness = 0.0f, metallic = 0.0f;
    material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness); // 粗さを取得
    material->Get(AI_MATKEY_METALLIC_FACTOR, metallic);   // 金属感を取得
    resultMaterial.roughness = roughness;
    resultMaterial.metallic = metallic;

    // マテリアルに関連するテクスチャパスを取得(テクスチャが存在する場合のみ)
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString textureName;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName);
        resultMaterial.texture = LoadTextureFile(textureName.C_Str()); // テクスチャのパスを保存
    }

    return resultMaterial; // 処理したマテリアルを返す
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
        throw std::runtime_error("画像ファイルの読み込みに失敗しました。: " + fileName);
    }

    return result;
}


