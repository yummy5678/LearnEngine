#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// glTFのファイル情報定義
// 拡張機能の情報については非対応

namespace glTF_Data
{
    struct Asset {
        std::string copyright;          // 著作権情報
        std::string generator;          // glTFを生成したツール名
        std::string version;            // glTFのバージョン   ※必須
        std::string minVersion;         // サポートされている最小バージョン
    };

    struct Scene {
        std::string name;               // シーンの名前 
        std::vector<int> nodes;         // シーンに含まれるノードのインデックスリスト
    };

    struct Node {
        std::string name;               // ノードの名前
        std::vector<int> children;      // このノードの子ノードのインデックスリスト
        glm::mat4 matrix;               // ノードの変換行列(4x4)
        glm::vec3 translation;          // ノードの位置ベクトル(3D)
        glm::quat rotation;             // ノードの回転を表すクォータニオン
        glm::vec3 scale;                // ノードのスケールベクトル(3D)
        int mesh;                       // このノードが参照するメッシュのインデックス
        int camera;                     // このノードが参照するカメラのインデックス
    };

    struct Mesh {
        std::string name;               // メッシュの名前
        std::vector<int> primitives;    // メッシュのプリミティブのインデックスリスト
        std::vector<float> weights;     // ウェイトのリスト(スキンアニメーション用)
    };

    struct Primitive {
        int mode;                       // プリミティブの描画モード(例: TRIANGLES)
        std::vector<int> attributes;    // 頂点属性のインデックスリスト
        std::vector<int> indices;       // 頂点インデックスのバッファ
        int material;                   // 使用するマテリアルのインデックス
        std::vector<int> modeTargets;   // モーフターゲットのインデックス(オプション)
    };

    struct Image {
        std::string name;               // 画像の名前
        std::string uri;                // 画像データのURI
        std::string mimeType;           // 画像データのMIMEタイプ

    };
        int bufferView;                 // 画像データが格納されているバッファビューのインデックス
    struct Sampler {
        std::string name;               // サンプラーの名前
        int magFilter;                  // 拡大フィルタリングの方法
        int minFilter;                  // 縮小フィルタリングの方法
        int wrapS;                      // S軸のラッピングモード
        int wrapT;                      // T軸のラッピングモード
    };

    struct Texture {
        std::string name;               // テクスチャの名前
        int sampler;                    // 使用するサンプラーのインデックス
        int source;                     // テクスチャ画像のインデックス
    };

    // マテリアル(モデル表面の材質)
    struct Material {
        std::string name;               // マテリアルの名前
        MetallicRoughness metallicRoughness; // 金属的および粗さの設定
        int normalTexture;              // 法線マップのインデックス
        int occlusionTexture;           // 陰影マップのインデックス
        int emissiveTexture;            // 自発光マップのインデックス
        glm::vec3 emissiveFactor;       // 自発光の強度(RGB)
        std::string alphaMode;          // アルファ(透明度)モード
        float alphaCutoff;              // アルファカットオフ値
        bool doubleSided;               // 両面描画のフラグ
    };

    struct MetallicRoughness {
        int baseColorTexture;           // ベースカラーのテクスチャ
        glm::vec4 baseColorFactor;      // ベースカラーの因子(RGBA)
        int metallicRoughnessTexture;   // 金属的および粗さのテクスチャ
        float metallicFactor;           // 金属的因子
        float roughnessFactor;          // 粗さ因子
    };

    struct TextureInfo {
        int index;                      // テクスチャのインデックス
        int texCoord;                   // テクスチャ座標のセット
        float scale;                    // テクスチャのスケール
        float strength;                 // テクスチャの強度
    };

    struct SceneCamera {
        std::string name;               // カメラの名前
        std::string type;               // カメラのタイプ(orthographicまたはperspective)
        OrthoGraphic orthographic;      // 平行投影に関する情報
        Perspective perspective;        // 透視投影に関する情報
    };

    struct OrthoGraphic {
        float xMag;                    // X方向のスケール
        float yMag;                    // Y方向のスケール
        float zFar;                    // カメラの遠近クリッピング面
        float zNear;                   // カメラの近接クリッピング面
    };

    struct Perspective {
        float aspectRatio;             // アスペクト比
        float yFov;                    // 垂直視野角
        float zFar;                    // カメラの遠近クリッピング面
        float zNear;                   // カメラの近接クリッピング面
    };

    struct Skin {
        std::string name;              // スキンの名前
        std::vector<float> inverseBindMatrices; // 逆バインド行列のリスト
        std::vector<int> joints;       // ボーンのインデックスリスト
        int skeleton;                  // スケルトンのノードのインデックス
    };

    struct Animation {
        std::string name;              // アニメーションの名前
        std::vector<int> channels;     // アニメーションチャンネルのリスト
        std::vector<int> samplers;     // サンプラーのリスト
    };


    //・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
    // 以下、ファイル読み込み時に使用する
    // データの配置を知る為の情報
    // (読み込みライブラリが使う為の情報なので作成不要)
    //・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
    struct Buffer {
        std::string name;           // バッファの名前
        size_t byteLength;          // バッファのバイト長
        std::string uri;            // バッファデータのURI
    };

    struct BufferView {
        std::string name;           // バッファビューの名前
        int buffer;                 // 対応するバッファのインデックス
        size_t byteOffset;          // バッファ内でのオフセット
        size_t byteLength;          // ビューのバイト長
        int target;                 // バッファビューのターゲット(例: ARRAY_BUFFER)
    };

    struct Accessor {
        std::string name;           // アクセサの名前
        int bufferView;             // データが格納されているバッファビューのインデックス
        size_t byteOffset;          // バッファ内でのオフセット
        int componentType;          // コンポーネントのデータ型
        bool normalized;            // 正規化フラグ
        int count;                  // 要素の数
        std::vector<float> max;     // 最大値のリスト
        std::vector<float> min;     // 最小値のリスト
        Sparse sparse;              // スパースデータの設定
    };

    struct Sparse {
        int count;                  // スパースデータの要素数
        int indices;                // インデックスデータのアクセサ
        int values;                 // 値データのアクセサ
    };
}
