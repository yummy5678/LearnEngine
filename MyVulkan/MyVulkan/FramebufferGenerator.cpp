#include "FramebufferGenerator.h"


FramebufferGenerator::FramebufferGenerator()
{
    m_ClassName = "FramebufferGenerator";
}

FramebufferGenerator::~FramebufferGenerator()
{

}

void FramebufferGenerator::Create(vk::Device logicalDevice, std::vector<vk::ImageView> imageViews, vk::RenderPass renderPass, vk::Extent2D extent)
{
    m_bCreated = true;
    // logicalDeviceが有効であるか確認
    if (!logicalDevice)
    {
        throw std::runtime_error("logicalDeviceが無効です！");
    }
    m_LogicalDevice = logicalDevice;

    //フレームバッファ作成情報を作成
    m_FramebufferInfos = CreateFramebufferInfos(imageViews, renderPass, extent);

    //作成情報からフレームバッファを作成
    for (const auto& info : m_FramebufferInfos)
    {
        m_Framebuffers.push_back(logicalDevice.createFramebuffer(info));
    }
}

void FramebufferGenerator::Destroy()
{
    //中身が作成されていないなら解放処理も行わない
    if (m_bCreated == false) return;
    m_bCreated = false;

    // フレームバッファの解放
    for (const auto& framebuffer : m_Framebuffers)
        m_LogicalDevice.destroyFramebuffer(framebuffer);
}

std::vector<vk::Framebuffer> FramebufferGenerator::GetFramebuffers()
{
    CheckCreated();
    return m_Framebuffers;
}

std::vector<vk::FramebufferCreateInfo> FramebufferGenerator::CreateFramebufferInfos(std::vector<vk::ImageView> imageViews, vk::RenderPass renderPass, vk::Extent2D extent)
{
    // renderPassが有効であるか確認
    if (!renderPass)
    {
        throw std::runtime_error("renderPassが無効です！");
    }

    // swapChainImagesが空でないことを確認
    if (imageViews.empty())
    {
        throw std::runtime_error("swapChainImagesが空です！");
    }

    std::vector<vk::FramebufferCreateInfo> framebufferInfos;
    framebufferInfos.reserve(imageViews.size());

    for (const auto& imageView : imageViews)
    {
        if (!imageView)
        {
            throw std::runtime_error("ImageViewが無効です！");
        }

        m_Attachments = { imageView };

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


