#pragma once
#include "VShaderConfigureBase.h"
class VTriangleShader :
    public VShaderConfigureBase
{
public:
    VTriangleShader();
    ~VTriangleShader();

    // �쐬�֐�(�e�b�Z�[�V�����V�F�[�_�[�͖��Ή�)
    void Create(vk::Device pLogicalDevice);

    std::vector<vk::DescriptorSetLayout>	GetDescriptorSetLayouts()	override;
    std::vector<vk::PushConstantRange>		GetPushConstantRanges()		override;
    vk::PipelineVertexInputStateCreateInfo	GetVertexInputState()		override;


private:





};

