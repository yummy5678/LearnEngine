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
    // logicalDevice���L���ł��邩�m�F
    if (!logicalDevice)
    {
        throw std::runtime_error("logicalDevice�������ł��I");
    }
    m_LogicalDevice = logicalDevice;

    //�t���[���o�b�t�@�쐬�����쐬
    m_FramebufferInfos = CreateFramebufferInfos(imageViews, renderPass, extent);

    //�쐬��񂩂�t���[���o�b�t�@���쐬
    for (const auto& info : m_FramebufferInfos)
    {
        m_Framebuffers.push_back(logicalDevice.createFramebuffer(info));
    }
}

void FramebufferGenerator::Destroy()
{
    //���g���쐬����Ă��Ȃ��Ȃ����������s��Ȃ�
    if (m_bCreated == false) return;
    m_bCreated = false;

    // �t���[���o�b�t�@�̉��
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
    // renderPass���L���ł��邩�m�F
    if (!renderPass)
    {
        throw std::runtime_error("renderPass�������ł��I");
    }

    // swapChainImages����łȂ����Ƃ��m�F
    if (imageViews.empty())
    {
        throw std::runtime_error("swapChainImages����ł��I");
    }

    std::vector<vk::FramebufferCreateInfo> framebufferInfos;
    framebufferInfos.reserve(imageViews.size());

    for (const auto& imageView : imageViews)
    {
        if (!imageView)
        {
            throw std::runtime_error("ImageView�������ł��I");
        }

        m_Attachments = { imageView };

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


