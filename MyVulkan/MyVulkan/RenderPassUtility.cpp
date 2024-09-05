#include "RenderPassUtility.h"

RenderpassGenerator::RenderpassGenerator()
{
    m_ClassName = "RenderpassGenerator";
}

RenderpassGenerator::~RenderpassGenerator()
{

}

void RenderpassGenerator::Create(vk::Device logicalDevice, const vk::SurfaceFormatKHR imageFormat)
{
    m_bCreated = true;
    m_LogicalDevice = logicalDevice;

    //auto colorAttachment = CreateColorAttachment(imageFormat);
    //auto colorAttachmentReference = CreateColorAttachmentReference();
    //auto subpass = CreateSubpass({}, { colorAttachmentReference }, {}, {}, {});
    //auto dependencies = CreateDependencies();
    //std::vector<vk::AttachmentDescription> a{ colorAttachment };
    //auto createInfo = CreateInfo(a, subpass, dependencies);
    //m_RenderPass = logicalDevice.createRenderPass(createInfo);
    m_RenderPass = CreateRenderpass(logicalDevice);
    //if (!renderPass)
    //{
    //    throw std::runtime_error("�����_�[�p�X�̍쐬�Ɏ��s���܂���!");
    //}
}

void RenderpassGenerator::Destroy()
{
    //�쐬�t���O�������Ă��Ȃ��ꍇ�͉���������s��Ȃ�
    if (m_bCreated == false) return;
    m_bCreated = false;

    // �����_�[�p�X�̉��
    m_LogicalDevice.destroyRenderPass(m_RenderPass);
}

vk::RenderPass RenderpassGenerator::GetRenderpass()
{
    CheckCreated();
    return m_RenderPass;
}

//void RenderpassGenerator::Release()
//{
    //m_pLogicalDevice->destroyRenderPass(m_RenderPass, nullptr);
//}

vk::AttachmentDescription RenderpassGenerator::CreateColorAttachment(const vk::SurfaceFormatKHR imageFormat)
{
    vk::AttachmentDescription attachment;
    // �J���[�o�b�t�@�A�^�b�`�����g�̋L�q
    attachment.flags;
    attachment.format = imageFormat.format;                          // �摜�t�H�[�}�b�g(�摜�쐬���̐ݒ�Ɠ����ɂ���)
    attachment.samples = vk::SampleCountFlagBits::e1;                // �}���`�T���v�����O�̃T���v����
    attachment.loadOp = vk::AttachmentLoadOp::eClear;                // �����_�[�p�X�̊J�n���ɃJ���[�o�b�t�@���N���A
    attachment.storeOp = vk::AttachmentStoreOp::eStore;              // �����_�[�p�X�̏I�����ɃJ���[�o�b�t�@��ۑ�
    attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;      // �X�e���V���o�b�t�@���g�p���Ȃ�
    attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;    // �X�e���V���o�b�t�@���g�p���Ȃ�
    attachment.initialLayout = vk::ImageLayout::eUndefined;          // �����_�[�p�X�J�n���̃��C�A�E�g
    attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;        // �����_�[�p�X�I�����̃��C�A�E�g(�\���p)
    
    return attachment;
}

vk::AttachmentReference RenderpassGenerator::CreateColorAttachmentReference()
{
    vk::AttachmentReference reference;
    // �T�u�p�X����`�挋�ʂ̐F���o�͂���A�^�b�`�����g���ǂꂩ���w�肵�܂�
    reference.attachment = 0;                                    // �A�^�b�`�����g�̃C���f�b�N�X
    reference.layout = vk::ImageLayout::eColorAttachmentOptimal; // �A�^�b�`�����g�̃��C�A�E�g
    return reference;
}

std::vector<vk::SubpassDescription> RenderpassGenerator::CreateSubpass(
    std::vector<vk::AttachmentReference> inputReferences,
    std::vector<vk::AttachmentReference> colorReferences,
    vk::AttachmentReference resolveReferences,
    vk::AttachmentReference depthStencilReferences,
    std::vector<uint32_t> preserveReferences)
{
    // �A�^�b�`�����g�Q��
    // �T�u�p�X����`�挋�ʂ̐F���o�͂���A�^�b�`�����g���ǂꂩ���w�肵�܂�
    m_ColorAttachmentRef.attachment = 0;                                    // �A�^�b�`�����g�̃C���f�b�N�X
    m_ColorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal; // �A�^�b�`�����g�̃��C�A�E�g

    // �T�u�p�X�̋L�q
    vk::SubpassDescription subpass;
    subpass.flags;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;  // �O���t�B�b�N�X�p�C�v���C���Ƀo�C���h
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount = 1;  // �J���[�o�b�t�@�A�^�b�`�����g�̐�
    subpass.pColorAttachments = &m_ColorAttachmentRef;  // �J���[�o�b�t�@�A�^�b�`�����g�̎Q��
    subpass.pResolveAttachments = nullptr;
    subpass.pDepthStencilAttachment = nullptr;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;

    return std::vector{ subpass };
}

std::vector<vk::SubpassDependency> RenderpassGenerator::CreateDependencies()
{

    std::vector<vk::SubpassDependency> dependencies;
    dependencies.resize(2); 

    // �T�u�p�X�ˑ��֌W�̋L�q
    // ���C�A�E�g�J�ڂ���������^�C�~���O���T�u�p�X�ˑ��֌W���g�p���Č��肷��K�v������܂�
    
    // VK_IMAGE_LAYOUT_UNDEFINED ���� VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL �ւ̕ϊ�
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;                           // �T�u�p�X�̃C���f�b�N�X (VK_SUBPASS_EXTERNAL = �����_�[�p�X�̊O�����Ӗ�������ʂȒl)
    dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;    // �p�C�v���C���X�e�[�W
    dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;            // �X�e�[�W�A�N�Z�X�}�X�N (�������A�N�Z�X)
    dependencies[0].dstSubpass = 0;                                             // �T�u�p�X�̃C���f�b�N�X
    dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; // �p�C�v���C���X�e�[�W
    dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite; // �X�e�[�W�A�N�Z�X�}�X�N
    dependencies[0].dependencyFlags = vk::DependencyFlags();                    // �ˑ��֌W�t���O
    
    // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL ���� VK_IMAGE_LAYOUT_PRESENT_SRC_KHR �ւ̕ϊ�
    dependencies[1].srcSubpass = 0;                                             // �T�u�p�X�̃C���f�b�N�X
    dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; // �p�C�v���C���X�e�[�W
    dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite; // �X�e�[�W�A�N�Z�X�}�X�N
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;                           // �T�u�p�X�̃C���f�b�N�X
    dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;    // �p�C�v���C���X�e�[�W
    dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;            // �X�e�[�W�A�N�Z�X�}�X�N
    dependencies[1].dependencyFlags = vk::DependencyFlags();                    // �ˑ��֌W�t���O

    return dependencies;
}

vk::RenderPassCreateInfo RenderpassGenerator::CreateInfo(
    std::vector<vk::AttachmentDescription>& colorAttachment, 
    std::vector<vk::SubpassDescription>& subpass, 
    std::vector<vk::SubpassDependency>& dependencies)
{
    // �����_�[�p�X�̋L�q
    m_RenderPassInfo.pNext;
    m_RenderPassInfo.flags;
    m_RenderPassInfo.attachmentCount    = colorAttachment.size();   // �A�^�b�`�����g�̐�
    m_RenderPassInfo.pAttachments       = colorAttachment.data();   // �A�^�b�`�����g�̋L�q
    m_RenderPassInfo.subpassCount       = subpass.size();           // �T�u�p�X�̐�
    m_RenderPassInfo.pSubpasses         = subpass.data();           // �T�u�p�X�̋L�q
    m_RenderPassInfo.dependencyCount    = dependencies.size();      // �T�u�p�X�ˑ��֌W�̐�
    m_RenderPassInfo.pDependencies      = dependencies.data();      // �T�u�p�X�ˑ��֌W�̋L�q

    return m_RenderPassInfo;
}

vk::RenderPass RenderpassGenerator::CreateRenderpass(vk::Device logicalDevice)
{
    vk::AttachmentDescription attachments[1];
    attachments[0].format = vk::Format::eR8G8B8A8Unorm;
    attachments[0].samples = vk::SampleCountFlagBits::e1;
    attachments[0].loadOp = vk::AttachmentLoadOp::eDontCare;
    attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
    attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    attachments[0].initialLayout = vk::ImageLayout::eUndefined;
    attachments[0].finalLayout = vk::ImageLayout::eGeneral;

    vk::AttachmentReference subpass0_attachmentRefs[1];
    subpass0_attachmentRefs[0].attachment = 0;
    subpass0_attachmentRefs[0].layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpasses[1];
    subpasses[0].pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpasses[0].colorAttachmentCount = 1;
    subpasses[0].pColorAttachments = subpass0_attachmentRefs;

    vk::RenderPassCreateInfo renderpassCreateInfo;
    renderpassCreateInfo.attachmentCount = 1;
    renderpassCreateInfo.pAttachments = attachments;
    renderpassCreateInfo.subpassCount = 1;
    renderpassCreateInfo.pSubpasses = subpasses;
    renderpassCreateInfo.dependencyCount = 0;
    renderpassCreateInfo.pDependencies = nullptr;

    return logicalDevice.createRenderPass(renderpassCreateInfo);
}
