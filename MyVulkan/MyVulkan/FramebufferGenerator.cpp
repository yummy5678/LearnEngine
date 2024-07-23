#include "FramebufferGenerator.h"

FramebufferGenerator::FramebufferGenerator(vk::Device logicalDevice, SwapchainGenerator& swapchain, vk::RenderPass renderPass)
{
    CreateFramebuffers(logicalDevice, swapchain.GetSwapChainImages(),renderPass,swapchain.Get2DExtent());
}

FramebufferGenerator::FramebufferGenerator(vk::Device logicalDevice, std::vector<SwapchainImage> swapChainImages, vk::RenderPass renderPass, vk::Extent2D extent)
{
    CreateFramebuffers(logicalDevice, swapChainImages, renderPass, extent);
}

FramebufferGenerator::~FramebufferGenerator()
{
}

std::vector<vk::Framebuffer> FramebufferGenerator::GetFramebuffers()
{
    return m_Framebuffers;
}

void FramebufferGenerator::CreateFramebuffers(vk::Device logicalDevice, std::vector<SwapchainImage> swapChainImages, vk::RenderPass renderPass, vk::Extent2D extent)
{
    // logicalDeviceが有効であるか確認
    if (!logicalDevice)
    {
        throw std::runtime_error("logicalDeviceが無効です！");
    }

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

    std::vector<vk::Framebuffer> framebuffers;
    framebuffers.reserve(swapChainImages.size());

    for (const auto& swapChainImage : swapChainImages)
    {
        if (!swapChainImage.imageView)
        {
            throw std::runtime_error("ImageViewが無効です！");
        }

        std::array<vk::ImageView, 1> attachments = { swapChainImage.imageView };

        vk::FramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.attachmentCount = (uint32_t)attachments.size();
        framebufferCreateInfo.pAttachments = attachments.data();
        framebufferCreateInfo.width = extent.width;
        framebufferCreateInfo.height = extent.height;
        framebufferCreateInfo.layers = 1;

        // vk::Framebufferを作成する
        vk::Framebuffer framebuffer = logicalDevice.createFramebuffer(framebufferCreateInfo);
        if (!framebuffer)
        {
            throw std::runtime_error("フレームバッファの作成に失敗しました！");
        }

        // vk::Framebufferをvk::UniqueFramebufferに変換する
        framebuffers.push_back(framebuffer);
    }

    m_Framebuffers = framebuffers;
}
