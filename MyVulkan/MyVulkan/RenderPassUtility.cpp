#include "RenderPassUtility.h"
using namespace VulkanUtility;


RenderpassGenerator::RenderpassGenerator(vk::Device logicalDevice, vk::SwapchainCreateInfoKHR swapchainInfo)
{
    CreateRenderpass(logicalDevice, swapchainInfo);


}

RenderpassGenerator::~RenderpassGenerator()
{
    Release();
}

void RenderpassGenerator::CreateRenderpass(vk::Device logicalDevice,vk::SwapchainCreateInfoKHR swapchainInfo)
{
    auto colorAttachment = CreateColorAttachment(swapchainInfo);
    auto subpass = CreateSubpass();
    auto dependencies = CreateDependencies();

    auto createInfo = CreateInfo(colorAttachment, subpass, dependencies);

    //�����_�[�p�X���쐬

    m_RenderPass = logicalDevice.createRenderPassUnique(createInfo);
    //if (!renderPass)
    //{
    //    throw std::runtime_error("�����_�[�p�X�̍쐬�Ɏ��s���܂���!");
    //}
}

vk::RenderPass RenderpassGenerator::GetRenderpass()
{
    return m_RenderPass.get();
}

void RenderpassGenerator::Release()
{
    //m_pLogicalDevice->destroyRenderPass(m_RenderPass, nullptr);
}

vk::AttachmentDescription* RenderpassGenerator::CreateColorAttachment(const vk::SwapchainCreateInfoKHR swapchainInfo)
{
    // �J���[�o�b�t�@�A�^�b�`�����g�̋L�q
    m_ColorAttachment.flags = {};
    m_ColorAttachment.format = swapchainInfo.imageFormat;       // �X���b�v�`�F�C���̃C���[�W�t�H�[�}�b�g
    m_ColorAttachment.samples = vk::SampleCountFlagBits::e1;      // �}���`�T���v�����O�̃T���v����
    m_ColorAttachment.loadOp = vk::AttachmentLoadOp::eClear;     // �����_�[�p�X�̊J�n���ɃJ���[�o�b�t�@���N���A
    m_ColorAttachment.storeOp = vk::AttachmentStoreOp::eStore;    // �����_�[�p�X�̏I�����ɃJ���[�o�b�t�@��ۑ�
    m_ColorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;  // �X�e���V���o�b�t�@���g�p���Ȃ�
    m_ColorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare; // �X�e���V���o�b�t�@���g�p���Ȃ�
    m_ColorAttachment.initialLayout = vk::ImageLayout::eUndefined;      // �����_�[�p�X�J�n�O�̃��C�A�E�g
    m_ColorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;  // �����_�[�p�X�I����̃��C�A�E�g�i�\���p�j
    
    return &m_ColorAttachment;
}

vk::SubpassDescription* RenderpassGenerator::CreateSubpass()
{
    // �A�^�b�`�����g�Q��
    // �T�u�p�X���烌���_�����O���ʂ̐F���o�͂���A�^�b�`�����g���ǂꂩ���w�肵�܂�
    m_ColorAttachmentRef.attachment = 0;                                        // �A�^�b�`�����g�̃C���f�b�N�X
    m_ColorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal; // �A�^�b�`�����g�̃��C�A�E�g

    // �T�u�p�X�̋L�q
    m_Subpass.flags = {};
    m_Subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;  // �O���t�B�b�N�X�p�C�v���C���Ƀo�C���h
    m_Subpass.inputAttachmentCount;
    m_Subpass.pInputAttachments = nullptr;
    m_Subpass.colorAttachmentCount = 1;  // �J���[�o�b�t�@�A�^�b�`�����g�̐�
    m_Subpass.pColorAttachments = &m_ColorAttachmentRef;  // �J���[�o�b�t�@�A�^�b�`�����g�̎Q��
    m_Subpass.pResolveAttachments = nullptr;
    m_Subpass.pDepthStencilAttachment = nullptr;
    m_Subpass.preserveAttachmentCount;
    m_Subpass.pPreserveAttachments = nullptr;

    return &m_Subpass;
}

std::vector<vk::SubpassDependency>* RenderpassGenerator::CreateDependencies()
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
    m_Dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;                       // �T�u�p�X�̃C���f�b�N�X
    m_Dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe; // �p�C�v���C���X�e�[�W
    m_Dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;        // �X�e�[�W�A�N�Z�X�}�X�N
    m_Dependencies[1].dependencyFlags = vk::DependencyFlags();                // �ˑ��֌W�t���O

    return &m_Dependencies;
}

vk::RenderPassCreateInfo RenderpassGenerator::CreateInfo(vk::AttachmentDescription* colorAttachment, vk::SubpassDescription* subpass, std::vector<vk::SubpassDependency>* dependencies)
{
    // �����_�[�p�X�̋L�q
    m_RenderPassInfo.pNext = nullptr;
    m_RenderPassInfo.flags = {};
    m_RenderPassInfo.attachmentCount = 1;                // �A�^�b�`�����g�̐�
    m_RenderPassInfo.pAttachments = colorAttachment; // �A�^�b�`�����g�̋L�q
    m_RenderPassInfo.subpassCount = 1;                // �T�u�p�X�̐�
    m_RenderPassInfo.pSubpasses = subpass;         // �T�u�p�X�̋L�q
    m_RenderPassInfo.dependencyCount = (uint32_t)dependencies->size();   // �T�u�p�X�ˑ��֌W�̐�
    m_RenderPassInfo.pDependencies = dependencies->data();      // �T�u�p�X�ˑ��֌W�̋L�q

    return m_RenderPassInfo;
}
