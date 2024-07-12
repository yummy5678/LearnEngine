#include "RenderPassUtility.h"
using namespace SwapChainUtility;

vk::UniqueRenderPass RenderPassUtility::createRenderPass(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    vk::Format              swapChainImageFormat    = chooseSwapSurfaceFormat(getSurfaceFormats(physicalDevice, surface)).format;

    // �J���[�o�b�t�@�A�^�b�`�����g�̋L�q
    vk::AttachmentDescription colorAttachment = {};
    colorAttachment.format          = swapChainImageFormat;             // �X���b�v�`�F�C���̃C���[�W�t�H�[�}�b�g
    colorAttachment.samples         = vk::SampleCountFlagBits::e1;      // �}���`�T���v�����O�̃T���v����
    colorAttachment.loadOp          = vk::AttachmentLoadOp::eClear;     // �����_�[�p�X�̊J�n���ɃJ���[�o�b�t�@���N���A
    colorAttachment.storeOp         = vk::AttachmentStoreOp::eStore;    // �����_�[�p�X�̏I�����ɃJ���[�o�b�t�@��ۑ�
    colorAttachment.stencilLoadOp   = vk::AttachmentLoadOp::eDontCare;  // �X�e���V���o�b�t�@���g�p���Ȃ�
    colorAttachment.stencilStoreOp  = vk::AttachmentStoreOp::eDontCare; // �X�e���V���o�b�t�@���g�p���Ȃ�
    colorAttachment.initialLayout   = vk::ImageLayout::eUndefined;      // �����_�[�p�X�J�n�O�̃��C�A�E�g
    colorAttachment.finalLayout     = vk::ImageLayout::ePresentSrcKHR;  // �����_�[�p�X�I����̃��C�A�E�g�i�\���p�j

    // �A�^�b�`�����g�Q��
    vk::AttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment   = 0;  // �A�^�b�`�����g�̃C���f�b�N�X
    colorAttachmentRef.layout       = vk::ImageLayout::eColorAttachmentOptimal;  // �A�^�b�`�����g�̃��C�A�E�g

    // �T�u�p�X�̋L�q
    vk::SubpassDescription subpass = {};
    subpass.pipelineBindPoint       = vk::PipelineBindPoint::eGraphics;  // �O���t�B�b�N�X�p�C�v���C���Ƀo�C���h
    subpass.colorAttachmentCount    = 1;  // �J���[�o�b�t�@�A�^�b�`�����g�̐�
    subpass.pColorAttachments       = &colorAttachmentRef;  // �J���[�o�b�t�@�A�^�b�`�����g�̎Q��

    // �T�u�p�X�ˑ��֌W�̋L�q
    vk::SubpassDependency dependency = {};
    dependency.srcSubpass           = VK_SUBPASS_EXTERNAL;  // �O���T�u�p�X�i�����_�[�p�X�̑O�j
    dependency.dstSubpass           = 0;  // �ŏ��̃T�u�p�X
    dependency.srcStageMask         = vk::PipelineStageFlagBits::eColorAttachmentOutput;  // �J���[�o�b�t�@�̏������݃X�e�[�W
    dependency.srcAccessMask        = vk::AccessFlagBits::eNone;  // �A�N�Z�X�}�X�N�Ȃ�
    dependency.dstStageMask         = vk::PipelineStageFlagBits::eColorAttachmentOutput;  // �J���[�o�b�t�@�̏������݃X�e�[�W
    dependency.dstAccessMask        = vk::AccessFlagBits::eColorAttachmentWrite;  // �J���[�o�b�t�@�̏������݃A�N�Z�X

    // �����_�[�p�X�̋L�q
    vk::RenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.attachmentCount  = 1;                // �A�^�b�`�����g�̐�
    renderPassInfo.pAttachments     = &colorAttachment; // �A�^�b�`�����g�̋L�q
    renderPassInfo.subpassCount     = 1;                // �T�u�p�X�̐�
    renderPassInfo.pSubpasses       = &subpass;         // �T�u�p�X�̋L�q
    renderPassInfo.dependencyCount  = 1;                // �T�u�p�X�ˑ��֌W�̐�
    renderPassInfo.pDependencies    = &dependency;      // �T�u�p�X�ˑ��֌W�̋L�q

    // �����_�[�p�X���쐬
    vk::UniqueRenderPass renderPass = logicalDevice.createRenderPassUnique(renderPassInfo);
    if (!renderPass)
    {
        throw std::runtime_error("�����_�[�p�X�̍쐬�Ɏ��s���܂���!");
    }

    return renderPass;
}
