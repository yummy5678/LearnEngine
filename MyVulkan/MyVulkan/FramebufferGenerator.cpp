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
    // logicalDevice���L���ł��邩�m�F
    if (!logicalDevice)
    {
        throw std::runtime_error("logicalDevice�������ł��I");
    }

    // renderPass���L���ł��邩�m�F
    if (!renderPass)
    {
        throw std::runtime_error("renderPass�������ł��I");
    }

    // swapChainImages����łȂ����Ƃ��m�F
    if (swapChainImages.empty())
    {
        throw std::runtime_error("swapChainImages����ł��I");
    }

    std::vector<vk::Framebuffer> framebuffers;
    framebuffers.reserve(swapChainImages.size());

    for (const auto& swapChainImage : swapChainImages)
    {
        if (!swapChainImage.imageView)
        {
            throw std::runtime_error("ImageView�������ł��I");
        }

        std::array<vk::ImageView, 1> attachments = { swapChainImage.imageView };

        vk::FramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.attachmentCount = (uint32_t)attachments.size();
        framebufferCreateInfo.pAttachments = attachments.data();
        framebufferCreateInfo.width = extent.width;
        framebufferCreateInfo.height = extent.height;
        framebufferCreateInfo.layers = 1;

        // vk::Framebuffer���쐬����
        vk::Framebuffer framebuffer = logicalDevice.createFramebuffer(framebufferCreateInfo);
        if (!framebuffer)
        {
            throw std::runtime_error("�t���[���o�b�t�@�̍쐬�Ɏ��s���܂����I");
        }

        // vk::Framebuffer��vk::UniqueFramebuffer�ɕϊ�����
        framebuffers.push_back(framebuffer);
    }

    m_Framebuffers = framebuffers;
}
