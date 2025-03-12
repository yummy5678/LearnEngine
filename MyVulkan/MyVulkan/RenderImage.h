#pragma once
#include "RenderTargetBase.h"
#include "VImage.h"
#include "DrawCommand.h"
#include "RenderFunction.h"


class RenderImage :
    public RenderTarget
{
public:
    RenderImage();
    ~RenderImage();

    vk::Extent2D	GetExtent() override;
    RenderingImageSet GetImageSet() override;
    vk::Format		GetColorFormat() override;
    vk::Format		GetDepthFormat() override;

    void Initialize(VmaAllocator* allocator, vk::ImageCreateInfo createInfo, vk::ImageAspectFlags aspectFlag);
    void AddDrawTask(std::shared_ptr<RenderFunction> function);
    void ExecuteDrawTask();

    void Cleanup();

private:
    vk::Device          m_LogicalDevice;
    vk::PhysicalDevice  m_PhysicalDevice;
    vk::Fence           m_Fence;

    VImage              m_ColorImage;
    VImage              m_DepthImage;
    vk::ImageCreateInfo m_ColorImageData;
    vk::ImageCreateInfo m_DepthImageData;
    RenderingImageSet   m_ImageSet;
    DrawCommand         m_DrawCommand;

    std::vector<std::shared_ptr<RenderFunction>> m_RenderFunctions;
};

