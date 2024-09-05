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

    m_LogicalDevice = logicalDevice;

    auto attachments = CreateAttachments(imageViews);

    //�t���[���o�b�t�@�쐬�����쐬
    m_FramebufferInfos = CreateFramebufferInfos(attachments, renderPass, extent);

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

std::vector<std::vector<vk::ImageView>> FramebufferGenerator::CreateAttachments(std::vector<vk::ImageView> imageViews)
{
    std::vector<std::vector<vk::ImageView>> attachments;
    attachments.reserve(imageViews.size());

    for (auto &imageView : imageViews)
    {
        //1�̃t���[���ɕۑ�����摜�̎��
        std::vector<vk::ImageView> attachment = { imageView };   //�����ł͍ŏI�o�͂̐F�f�[�^�̂�  
        attachments.push_back(attachment);
    }

    return attachments;
}

std::vector<vk::Framebuffer> FramebufferGenerator::GetFramebuffers()
{
    CheckCreated();
    return m_Framebuffers;
}

std::vector<vk::FramebufferCreateInfo> FramebufferGenerator::CreateFramebufferInfos(std::vector<std::vector<vk::ImageView>>& attachments, vk::RenderPass renderPass, vk::Extent2D extent)
{
    std::vector<vk::FramebufferCreateInfo> framebufferInfos;
    framebufferInfos.resize(attachments.size());

    for (int i = 0;i < framebufferInfos.size();i++)
    {
        vk::FramebufferCreateInfo createInfo;
        createInfo.pNext;
        createInfo.flags;
        createInfo.renderPass       = renderPass;                       //�ˑ����Ă��郌���_�[�p�X
        createInfo.attachmentCount  = (uint32_t)attachments[i].size();  //�摜�̎�ސ�
        createInfo.pAttachments     = attachments[i].data();            //�摜�̃f�[�^
        createInfo.width            = extent.width;                     //�摜�̕�
        createInfo.height           = extent.height;                    //�摜�̍���
        createInfo.layers           = 1;                                //���_�̐�

        // vk::Framebuffer��vk::UniqueFramebuffer�ɕϊ�����
        framebufferInfos[i] = createInfo;
    }

    return framebufferInfos;
}


