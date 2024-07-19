#include "RenderPassUtility.h"
using namespace VulkanUtility;

vk::RenderPassCreateInfo VulkanCreate::GetRenderPassInfo(vk::SwapchainCreateInfoKHR* swapchainInfo)
{
    // �J���[�o�b�t�@�A�^�b�`�����g�̋L�q
    static vk::AttachmentDescription colorAttachment;
    colorAttachment.flags           = {};
    colorAttachment.format          = swapchainInfo->imageFormat;       // �X���b�v�`�F�C���̃C���[�W�t�H�[�}�b�g
    colorAttachment.samples         = vk::SampleCountFlagBits::e1;      // �}���`�T���v�����O�̃T���v����
    colorAttachment.loadOp          = vk::AttachmentLoadOp::eClear;     // �����_�[�p�X�̊J�n���ɃJ���[�o�b�t�@���N���A
    colorAttachment.storeOp         = vk::AttachmentStoreOp::eStore;    // �����_�[�p�X�̏I�����ɃJ���[�o�b�t�@��ۑ�
    colorAttachment.stencilLoadOp   = vk::AttachmentLoadOp::eDontCare;  // �X�e���V���o�b�t�@���g�p���Ȃ�
    colorAttachment.stencilStoreOp  = vk::AttachmentStoreOp::eDontCare; // �X�e���V���o�b�t�@���g�p���Ȃ�
    colorAttachment.initialLayout   = vk::ImageLayout::eUndefined;      // �����_�[�p�X�J�n�O�̃��C�A�E�g
    colorAttachment.finalLayout     = vk::ImageLayout::ePresentSrcKHR;  // �����_�[�p�X�I����̃��C�A�E�g�i�\���p�j

    // �A�^�b�`�����g�Q��
    static vk::AttachmentReference colorAttachmentRef;
    colorAttachmentRef.attachment   = 0;                                        // �A�^�b�`�����g�̃C���f�b�N�X
    colorAttachmentRef.layout       = vk::ImageLayout::eColorAttachmentOptimal; // �A�^�b�`�����g�̃��C�A�E�g

    // �T�u�p�X�̋L�q
    static vk::SubpassDescription subpass;
    subpass.flags                   = {};
    subpass.pipelineBindPoint       = vk::PipelineBindPoint::eGraphics;  // �O���t�B�b�N�X�p�C�v���C���Ƀo�C���h
    subpass.inputAttachmentCount;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount    = 1;  // �J���[�o�b�t�@�A�^�b�`�����g�̐�
    subpass.pColorAttachments       = &colorAttachmentRef;  // �J���[�o�b�t�@�A�^�b�`�����g�̎Q��
    subpass.pResolveAttachments = nullptr;
    subpass.pDepthStencilAttachment = nullptr;
    subpass.preserveAttachmentCount;
    subpass.pPreserveAttachments = nullptr;

    // �T�u�p�X�ˑ��֌W�̋L�q
    // ���C�A�E�g�J�ڂ���������^�C�~���O���T�u�p�X�ˑ��֌W���g�p���Č��肷��K�v������܂�
    static std::array<vk::SubpassDependency, 2> dependencies;

    // VK_IMAGE_LAYOUT_UNDEFINED ���� VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL �ւ̕ϊ�
    // �J�ڂ͎��̌�Ŕ�������K�v������܂�...
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;                       // �T�u�p�X�̃C���f�b�N�X (VK_SUBPASS_EXTERNAL = �����_�[�p�X�̊O�����Ӗ�������ʂȒl)
    dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe; // �p�C�v���C���X�e�[�W
    dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;        // �X�e�[�W�A�N�Z�X�}�X�N (�������A�N�Z�X)
    // �������A���̑O�Ŕ�������K�v������܂�...
    dependencies[0].dstSubpass = 0;                                         // �T�u�p�X�̃C���f�b�N�X
    dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; // �p�C�v���C���X�e�[�W
    dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite; // �X�e�[�W�A�N�Z�X�}�X�N
    dependencies[0].dependencyFlags = vk::DependencyFlags();                // �ˑ��֌W�t���O

    // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL ���� VK_IMAGE_LAYOUT_PRESENT_SRC_KHR �ւ̕ϊ�
    // �J�ڂ͎��̌�Ŕ�������K�v������܂�...
    dependencies[1].srcSubpass = 0;                                         // �T�u�p�X�̃C���f�b�N�X
    dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; // �p�C�v���C���X�e�[�W
    dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite; // �X�e�[�W�A�N�Z�X�}�X�N
    // �������A���̑O�Ŕ�������K�v������܂�...
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;                       // �T�u�p�X�̃C���f�b�N�X
    dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe; // �p�C�v���C���X�e�[�W
    dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;        // �X�e�[�W�A�N�Z�X�}�X�N
    dependencies[1].dependencyFlags = vk::DependencyFlags();                // �ˑ��֌W�t���O


    // �����_�[�p�X�̋L�q
    vk::RenderPassCreateInfo renderPassInfo;
    renderPassInfo.pNext = nullptr;
    renderPassInfo.flags = {};
    renderPassInfo.attachmentCount  = 1;                // �A�^�b�`�����g�̐�
    renderPassInfo.pAttachments = colorAttachment.get(); // �A�^�b�`�����g�̋L�q
    renderPassInfo.subpassCount     = 1;                // �T�u�p�X�̐�
    renderPassInfo.pSubpasses       = &subpass;         // �T�u�p�X�̋L�q
    renderPassInfo.dependencyCount  = (uint32_t)dependencies.size();   //             // �T�u�p�X�ˑ��֌W�̐�
    renderPassInfo.pDependencies    = dependencies.data();      // �T�u�p�X�ˑ��֌W�̋L�q



    return renderPassInfo;


}
    // �����_�[�p�X���쐬
    //vk::UniqueRenderPass renderPass = logicalDevice.createRenderPassUnique(renderPassInfo);
    //if (!renderPass)
    //{
    //    throw std::runtime_error("�����_�[�p�X�̍쐬�Ɏ��s���܂���!");
    //}

    //return renderPass;