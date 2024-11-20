#pragma once
#include <vulkan/vulkan.hpp>
#include "MeshObject.h"



//・・・・・・・・・・・・・・・・・・・・・・・・
// パイプラインに渡す情報の定義
//・・・・・・・・・・・・・・・・・・・・・・・・

// Vertex 構造体の情報を定義
//glm::vec3 position;		// 頂点座標 (x, y, z)
//glm::vec3 normal;			// 法線ベクトル(面の向き)
//glm::vec2 textureCoord;	// テクスチャ座標 (u, v)

namespace VertexInputBinding
{
     // バインディングの定義   //後でテンプレート化する
     constexpr std::array<vk::VertexInputBindingDescription,1> bindingDescriptions = 
     {
         vk::VertexInputBindingDescription
         {
            0,                              // binding  
            sizeof(Vertex),                 // stride   
            vk::VertexInputRate::eVertex    // inputRate
         }    
     };

    // 入力属性の定義
    constexpr std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = {
        // 座標
        vk::VertexInputAttributeDescription{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) },

        // 法線
       vk::VertexInputAttributeDescription{ 0, 1, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal) },

        // テクスチャ座標
       vk::VertexInputAttributeDescription{ 0, 2, vk::Format::eR32G32Sfloat, offsetof(Vertex, textureCoord) }

    };

    // パイプライン頂点入力状態の作成
    static vk::PipelineVertexInputStateCreateInfo& GetVertexInputInfo()
    {
        vk::PipelineVertexInputStateCreateInfo resultInfo;
        resultInfo.setVertexBindingDescriptions(bindingDescriptions);
        resultInfo.setVertexAttributeDescriptions(attributeDescriptions);
        return resultInfo;          
    };
};



// 頂点情報からプリミティブ(面)の組み立て方の定義
//constexpr vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = [] {//グラフィクスパイプラインの作成に必要
//    vk::PipelineInputAssemblyStateCreateInfo info;
//    info.topology = vk::PrimitiveTopology::eTriangleList;   // トポロジー(三角形リスト)
//    info.primitiveRestartEnable = VK_FALSE;                 // プリミティブ再開を無効にする
//    return info;
//    }();



///////////////////////////////////
// プッシュ定数
///////////////////////////////////
// Model
//constexpr std::array<vk::PushConstantRange, 1> pushConstantModelRange = {
//    // プッシュ定数に作成情報とかは必要ない
//    vk::PushConstantRange{
//    vk::ShaderStageFlagBits::eVertex,	// 渡したいシェーダーステージ
//    0,								    // 渡したデータからどの位置のデータを見るか
//    sizeof(Transform)					// 渡したいデータのサイズ
//    }
//};


















