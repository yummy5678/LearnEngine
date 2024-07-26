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
    // logicalDevice���L���ł��邩�m�F
    if (!logicalDevice)
    {
        throw std::runtime_error("logicalDevice�������ł��I");
    }
    m_LogicalDevice = logicalDevice;

    //�쐬�����쐬
    m_FramebufferInfos = CreateFramebufferInfos(swapChainImages, renderPass, extent);

    //�쐬��񂩂�t���[���o�b�t�@���쐬
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

    std::vector<vk::FramebufferCreateInfo> framebufferInfos;
    framebufferInfos.reserve(swapChainImages.size());

    for (const auto& swapChainImage : swapChainImages)
    {
        if (!swapChainImage.imageView)
        {
            throw std::runtime_error("ImageView�������ł��I");
        }

        m_Attachments = { swapChainImage.imageView };

        vk::FramebufferCreateInfo createInfo;
        createInfo.renderPass       = renderPass;
        createInfo.attachmentCount  = (uint32_t)m_Attachments.size();
        createInfo.pAttachments     = m_Attachments.data();
        createInfo.width            = extent.width;
        createInfo.height           = extent.height;
        createInfo.layers           = 1;

        // vk::Framebuffer��vk::UniqueFramebuffer�ɕϊ�����
        framebufferInfos.push_back(createInfo);
    }

    return framebufferInfos;
}


