#include "FramebufferGenerator.h"
#include "VulkanUtility.h"

FramebufferGenerator::FramebufferGenerator()
{
    m_ClassName = "FramebufferGenerator";
}

FramebufferGenerator::~FramebufferGenerator()
{
    Destroy(m_LogicalDevice);
}

void FramebufferGenerator::Create(vk::Device logicalDevice, std::vector<SwapchainImage> swapChainImages, vk::RenderPass renderPass, vk::Extent2D extent)
{
    m_bCreated = true;
    // logicalDeviceが有効であるか確認
    if (!logicalDevice)
    {
        throw std::runtime_error("logicalDeviceが無効です！");
    }
    m_LogicalDevice = logicalDevice;

    //作成情報を作成
    m_FramebufferInfos = CreateFramebufferInfos(swapChainImages, renderPass, extent);

    //作成情報からフレームバッファを作成
    for (const auto& info : m_FramebufferInfos)
    {
        m_Framebuffers.push_back(logicalDevice.createFramebuffer(info));
    }
}

void FramebufferGenerator::Destroy(vk::Device logicalDevice)
{
    for (const auto& framebuffer : m_Framebuffers)
        vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
}

std::vector<vk::Framebuffer> FramebufferGenerator::GetFramebuffers()
{
    CheckCreated();
    return m_Framebuffers;
    ;
}

std::vector<vk::FramebufferCreateInfo> FramebufferGenerator::CreateFramebufferInfos(std::vector<SwapchainImage> swapChainImages, vk::RenderPass renderPass, vk::Extent2D extent)
{
    // renderPassが有効であるか確認
    if (!renderPass)
    {
        throw std::runtime_error("renderPassが無効です！");
    }

    // swapChainImagesが空でないことを確認
    if (swapChainImages.empty())
    {
        throw std::runtime_error("swapChainImagesが空です！");
    }

    std::vector<vk::FramebufferCreateInfo> framebufferInfos;
    framebufferInfos.reserve(swapChainImages.size());

    for (const auto& swapChainImage : swapChainImages)
    {
        if (!swapChainImage.imageView)
        {
            throw std::runtime_error("ImageViewが無効です！");
        }

        m_Attachments = { swapChainImage.imageView };

        vk::FramebufferCreateInfo createInfo;
        createInfo.renderPass       = renderPass;
        createInfo.attachmentCount  = (uint32_t)m_Attachments.size();
        createInfo.pAttachments     = m_Attachments.data();
        createInfo.width            = extent.width;
        createInfo.height           = extent.height;
        createInfo.layers           = 1;

        // vk::Framebufferをvk::UniqueFramebufferに変換する
        framebufferInfos.push_back(createInfo);
    }

    return framebufferInfos;
}


