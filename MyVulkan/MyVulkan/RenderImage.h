#pragma once
#include "RenderTargetBase.h"
#include "DrawCommand.h"
#include "RenderFunction.h"


class RenderImage :
    public RenderTarget
{
public:
    RenderImage();
    ~RenderImage();

    vk::Extent2D	    GetExtent() override;
    RenderingImageSet   GetImageSet() override;
    vk::Format		    GetColorFormat() override;
    vk::Format		    GetDepthFormat() override;

    void Initialize(VmaAllocator* allocator, vk::Extent2D extent);
    void AddDrawTask(std::shared_ptr<RenderFunction> function);
    void ExecuteDrawTask();

    void Cleanup();

private:
    vk::Device          m_LogicalDevice;
    vk::PhysicalDevice  m_PhysicalDevice;
    vk::Fence           m_Fence;

    VTextureBuffer      m_ColorImage;
    VTextureBuffer      m_DepthImage;
    vk::Extent2D        m_ImageExtent;
    const vk::Format          m_ImageFormat;
    const vk::ImageAspectFlags m_ImageAspectFlag;
    const vk::ImageUsageFlags m_ColorImageUsage;
    const vk::ImageUsageFlags m_DepthImageUsage;

    RenderingImageSet   m_ImageSet;
    DrawCommand         m_DrawCommand;

    std::vector<std::shared_ptr<RenderFunction>> m_RenderFunctions;


    vk::ImageCreateInfo GetImageCreateInfo(vk::Extent2D extent, vk::Format format, vk::ImageUsageFlags usage);
};

