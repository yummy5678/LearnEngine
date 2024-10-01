#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>          // 頂点やマテリアル用のベクトルや行列のライブラリ


// ※パイプラインにこれらの情報を渡すための定義は"PipelineInputUtility"に記載
// 頂点情報を格納するための構造体
struct Vertex
{
	glm::vec3 position;		// 頂点座標 (x, y, z)
	glm::vec3 normal;		// 法線ベクトル(面の向き)
	glm::vec2 textureCoord; // テクスチャ座標 (u, v)
};

// メッシュ(頂点とインデックス)の情報を格納するための構造体
struct Mesh
{
	std::vector<Vertex>		vertices;	// 頂点のリスト
	std::vector<uint32_t>	indices;	// 頂点のインデックスリスト(頂点の描画順序)
};

// マテリアル(色やテクスチャ)の情報を格納するための構造体
struct Material
{
	glm::vec4 baseColor;		// 基本色 (r, g, b, a)
	float metallic;				// 金属感
	float roughness;			// 表面の粗さ

	std::string texturePath;	// テクスチャ画像のファイルパス
};

//struct Animation
//{
//	std::vector<Bone>;
//		std::vector<Skin>;
//		std::vector<Keyframe>;
//};

struct MeshObject
{
	std::vector<Mesh>		meshes;		// オブジェクトの形の情報
	std::vector<Material>	materials;	// オブジェクトの表面の見た目の情報
	//Animation animation;	// オブジェクトのアニメーション情報
};



//頂点バッファとインデックスバッファについて