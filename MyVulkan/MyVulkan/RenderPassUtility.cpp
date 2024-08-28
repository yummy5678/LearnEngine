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

    auto colorAttachment = CreateColorAttachment(imageFormat);
    auto subpass = CreateSubpass();
    auto dependencies = CreateDependencies();

    auto createInfo = CreateInfo(colorAttachment, subpass, dependencies);
    m_RenderPass = logicalDevice.createRenderPass(createInfo);
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

std::vector<vk::AttachmentDescription> RenderpassGenerator::CreateColorAttachment(const vk::SurfaceFormatKHR imageFormat)
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
    attachment.initialLayout = vk::ImageLayout::eUndefined;          // �����_�[�p�X�J�n�O�̃��C�A�E�g
    attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;        // �����_�[�p�X�I����̃��C�A�E�g�i�\���p�j
    
    return std::vector{ attachment };
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
    std::vector<vk::AttachmentReference> inputReferences = {},
    std::vector<vk::AttachmentReference> colorReferences = {},
    std::vector<vk::AttachmentReference> resolveReferences = {},
    std::vector<vk::AttachmentReference> depthStencilReferences = {},
    std::vector<vk::AttachmentReference> preserveReferences = {})
{
    // �A�^�b�`�����g�Q��
    // �T�u�p�X����`�挋�ʂ̐F���o�͂���A�^�b�`�����g���ǂꂩ���w�肵�܂�
    //m_ColorAttachmentRef.attachment = 0;                                    // �A�^�b�`�����g�̃C���f�b�N�X
    //m_ColorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal; // �A�^�b�`�����g�̃��C�A�E�g

    // �T�u�p�X�̋L�q
    vk::SubpassDescription subpass;
    subpass.flags;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;  // �O���t�B�b�N�X�p�C�v���C���Ƀo�C���h
    subpass.inputAttachmentCount;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount = colorReferences.size();  // �J���[�o�b�t�@�A�^�b�`�����g�̐�
    subpass.pColorAttachments = colorReferences.data();  // �J���[�o�b�t�@�A�^�b�`�����g�̎Q��
    subpass.resolveAttachmentCount
    subpass.pResolveAttachments = resolveReferences.data();
    subpass.pDepthStencilAttachment = nullptr;
    subpass.preserveAttachmentCount = preserveReferences.size();
    subpass.pPreserveAttachments = preserveReferences.data();

    return std::vector{ subpass };
}

std::vector<vk::SubpassDependency> RenderpassGenerator::CreateDependencies()
{
    // �T�u�p�X�ˑ��֌W�̋L�q
    // ���C�A�E�g�J�ڂ���������^�C�~���O���T�u�p�X�ˑ��֌W���g�p���Č��肷��K�v������܂�
    m_Dependencies.resize(2);
    // VK_IMAGE_LAYOUT_UNDEFINED ���� VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL �ւ̕ϊ�
    // �J�ڂ͎��̌�Ŕ�������K�v������܂�...
    m_Dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;                       // �T�u�p�X�̃C���f�b�N�X (VK_SUBPASS_EXTERNAL = �����_�[�p�X�̊O�����Ӗ�������ʂȒl)
    m_Dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe; // �p�C�v���C���X�e�[�W
    m_Dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;        // �X�e�[�W�A�N�Z�X�}�X�N (�������A�N�Z�X)
    // �������A���̑O�Ŕ�������K�v������܂�...
    m_Dependencies[0].dstSubpass = 0;                                         // �T�u�p�X�̃C���f�b�N�X
    m_Dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; // �p�C�v���C���X�e�[�W
    m_Dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite; // �X�e�[�W�A�N�Z�X�}�X�N
    m_Dependencies[0].dependencyFlags = vk::DependencyFlags();                // �ˑ��֌W�t���O
    
    // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL ���� VK_IMAGE_LAYOUT_PRESENT_SRC_KHR �ւ̕ϊ�
    // �J�ڂ͎��̌�Ŕ�������K�v������܂�...
    m_Dependencies[1].srcSubpass = 0;                                         // �T�u�p�X�̃C���f�b�N�X
    m_Dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; // �p�C�v���C���X�e�[�W
    m_Dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite; // �X�e�[�W�A�N�Z�X�}�X�N
    // �������A���̑O�Ŕ�������K�v������܂�...
    m_Dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;                         // �T�u�p�X�̃C���f�b�N�X
    m_Dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;  // �p�C�v���C���X�e�[�W
    m_Dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;          // �X�e�[�W�A�N�Z�X�}�X�N
    m_Dependencies[1].dependencyFlags = vk::DependencyFlags();                  // �ˑ��֌W�t���O

    return m_Dependencies;
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
