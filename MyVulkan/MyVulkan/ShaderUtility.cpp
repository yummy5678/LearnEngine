#include "ShaderUtility.h"

vk::ShaderModule ShaderUtility::createShaderModule(const std::vector<char>& code, vk::Device logicalDevice)
{
    // シェーダーモジュール作成情報の設定
    vk::ShaderModuleCreateInfo shaderModuleCreateInfo(
        {},                                             // VkStructureType は自動的に設定される
        code.size(),                                    // コードのサイズ
        reinterpret_cast<const uint32_t*>(code.data())  // コードのポインタをuint32_t*にキャスト
    );

    // シェーダーモジュールの作成
    vk::ShaderModule shaderModule;
    shaderModule = logicalDevice.createShaderModule(shaderModuleCreateInfo);
    if (!shaderModule)
    {
        throw std::runtime_error("シェーダーモジュールの作成に失敗しました！");
    }

    return shaderModule;
}