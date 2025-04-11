#pragma once
#include "RenderTargetBase.h"
#include "DrawCommand.h"
#include "RenderFunction.h"
#include "ImageBuffer.h"
#include "WriteImage.h"

class RenderImage :
    public RenderingTarget
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

    void WriteImage(std::string fileName);

    void Cleanup();

private:
    vk::Device          m_LogicalDevice;
    vk::PhysicalDevice  m_PhysicalDevice;
    vk::Fence           m_Fence;

    VImageBuffer        m_ColorImage;
    VImageBuffer        m_DepthImage;
    vk::Extent2D        m_ImageExtent;
    //const vk::Format    m_ColorImageFormat;
    //const vk::Format    m_DepthImageFormat;
    //const vk::ImageAspectFlags m_ImageAspectFlag;
    //const vk::ImageUsageFlags m_ColorImageUsage;
    //const vk::ImageUsageFlags m_DepthImageUsage;

    RenderingImageSet   m_ImageSet;
    DrawCommand         m_DrawCommand;

    std::vector<std::shared_ptr<RenderFunction>> m_RenderFunctions;


    vk::ImageCreateInfo GetImageCreateInfo(vk::Extent2D extent, vk::Format format, vk::ImageUsageFlags usage);
};

