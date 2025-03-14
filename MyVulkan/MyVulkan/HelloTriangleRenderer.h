#pragma once
#include "RendererBase.h"
#include "VTriangleShader.h"
#include "RenderTargetBase.h"
#include "RenderFunction.h"

class HelloTriangleRenderer : public RendererBase
{
public:
    HelloTriangleRenderer(VulkanInitializer& initializer);
    ~HelloTriangleRenderer();

    virtual vk::Pipeline			GetPipeline() override;
    virtual vk::PipelineLayout		GetPipelineLayout() override;


    void Initialize(RenderTarget* renderere);
    std::shared_ptr<RenderFunction> GetRenderFunction();


private:
    VTriangleShader m_Shader;


};

