#pragma once
#include "VShaderConfigureBase.h"
class VTriangleShader :
    public VShaderConfigureBase
{
public:
    VTriangleShader();
    ~VTriangleShader();

    // 作成関数(テッセーションシェーダーは未対応)
    void Create(vk::Device pLogicalDevice);

    std::vector<vk::DescriptorSetLayout>	GetDescriptorSetLayouts()	override;
    std::vector<vk::PushConstantRange>		GetPushConstantRanges()		override;
    vk::PipelineVertexInputStateCreateInfo	GetVertexInputState()		override;


private:





};

