#pragma once
#include <vulkan/vulkan.hpp>
#include "ModelLoder.h"

//・・・・・・・・・・・・・・・・・・・・・・・・
// パイプラインに渡す情報の定義
//・・・・・・・・・・・・・・・・・・・・・・・・

// Vertex 構造体の情報を定義
//glm::vec3 position;		// 頂点座標 (x, y, z)
//glm::vec3 normal;			// 法線ベクトル(面の向き)
//glm::vec2 textureCoord;	// テクスチャ座標 (u, v)

// バインディングの定義   //後でテンプレート化する
constexpr vk::VertexInputBindingDescription bindingDescription = [] {
    vk::VertexInputBindingDescription binding;
    binding.binding = 0;
    binding.stride = sizeof(Vertex);
    binding.inputRate = vk::VertexInputRate::eVertex;
    return binding;
}();

// 入力属性の定義
constexpr std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = [] {
    std::array<vk::VertexInputAttributeDescription, 3> attribute;

    // 座標
    attribute[0] = { 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) };

    // 法線
    attribute[1] = { 0, 1, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal) };

    // テクスチャ座標
    attribute[2] = { 0, 2, vk::Format::eR32G32Sfloat, offsetof(Vertex, textureCoord) };

    return attribute;
}();

// パイプライン頂点入力状態の作成
constexpr vk::PipelineVertexInputStateCreateInfo vertexInputInfo = [] {
    vk::PipelineVertexInputStateCreateInfo info;
    info.vertexBindingDescriptionCount = 1;
    info.pVertexBindingDescriptions = &bindingDescription;
    info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    info.pVertexAttributeDescriptions = attributeDescriptions.data();
    return info;
}();


// 頂点情報からプリミティブ(面)の組み立て方の定義
constexpr vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = [] {
    vk::PipelineInputAssemblyStateCreateInfo info;
    info.topology = vk::PrimitiveTopology::eTriangleList;   // トポロジー(三角形リスト)
    info.primitiveRestartEnable = VK_FALSE;                 // プリミティブ再開を無効にする
    return info;
    }();
