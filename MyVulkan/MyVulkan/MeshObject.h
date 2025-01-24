#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>          // 頂点やマテリアル用のベクトルや行列のライブラリ
#include <glm/fwd.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

// ※パイプラインにこれらの情報を渡すための定義は"PipelineInputUtility"に記載

struct SamplerInfo	// 各モードの詳細については未作成
{
	uint32_t	magFilter = 0;	// テクスチャを拡大する際のフィルタリング方法
	uint32_t	minFilter = 0;	// テクスチャを縮小する際のフィルタリング方法
	uint32_t	wrapS = 0;		// テクスチャのS方向（U軸）の境界処理
	uint32_t	wrapT = 0;		// テクスチャのT方向（V軸）の境界処理
};			
					


struct Texture
{
	uint8_t* data = 0;			// RAM上の画像データのポインタ
	uint32_t width = 0;			// 画像の幅
	uint32_t height = 0;		// 画像の高さ
	uint32_t channel = 0;		// 1画素に含まれるチャンネル数	
};

// マテリアル(色やテクスチャ)の情報を格納するための構造体
struct Material
{
	glm::vec4	baseColor		= glm::vec4(1.0f);	// 基本色 (r, g, b, a)
	float		metallic		= 0.0f;				// 金属感
	float		roughness		= 0.0f;				// 表面の粗さ

	Texture		texture			= Texture();		// テクスチャ画像のデータ
	// SamplerInfo	samplerInfo;
};


// 頂点情報を格納するための構造体
struct Vertex
{
	glm::vec3 position = glm::vec3(0);		// 頂点座標 (x, y, z)
	glm::vec3 normal   = glm::vec3(0);		// 法線ベクトル(面の向き)
	glm::vec2 textureCoord = glm::vec2(0); // テクスチャ座標 (u, v)
};

// メッシュ(頂点とインデックス)の情報を格納するための構造体
struct Mesh
{
	std::vector<Vertex>		vertices;	// 頂点のリスト
	std::vector<uint32_t>	indices;	// 頂点のインデックスリスト(頂点の描画順序)
};

//struct Animation
//{
//	std::vector<Bone>;
//	std::vector<Skin>;
//	std::vector<Keyframe>;
//};

using Transform = glm::mat4;

// 名前の初期値(名前が未設定の時用)
static const char* DafaultObjectName = "Undefined";

struct MeshObject
{
	std::string name		= DafaultObjectName;
	Transform	transform	= Transform(1.0f);

	Mesh		mesh		= Mesh();		// オブジェクトの形の情報
	Material	material	= Material();	// オブジェクトの表面の見た目の情報
	//Animation animation;	// オブジェクトのアニメーション情報
};



//頂点バッファとインデックスバッファについて