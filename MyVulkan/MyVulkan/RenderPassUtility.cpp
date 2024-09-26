#include "RenderPassUtility.h"

RenderpassGenerator::RenderpassGenerator()
{
    m_ClassName = "RenderpassGenerator";
}

RenderpassGenerator::~RenderpassGenerator()
{

}

void RenderpassGenerator::LoadShader(vk::Device logicalDevice, vk::Format imageFomat)
{
    m_bCreated = true;
    m_LogicalDevice = logicalDevice;

    m_RenderPass = CreateRenderpass(logicalDevice, imageFomat);
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


vk::RenderPass RenderpassGenerator::CreateRenderpass(vk::Device logicalDevice, vk::Format imageFormat)
{
    // �A�^�b�`�����g�̐ݒ�
    vk::AttachmentDescription colorAttachment = {};
    colorAttachment.setFormat(chooseSupportedFormat(
        { vk::Format::eR8G8B8A8Unorm },
        vk::ImageTiling::eOptimal,
        vk::FormatFeatureFlagBits::eColorAttachment));
    colorAttachment.setSamples(vk::SampleCountFlagBits::e1); // �}���`�T���v�����O�̃T���v����
    colorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear); // �����_�[�p�X�J�n���ɃN���A
    colorAttachment.setStoreOp(vk::AttachmentStoreOp::eDontCare); // �I�����ɕۑ����Ȃ�
    colorAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare); // �X�e���V�����g�p���Ȃ�
    colorAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
    colorAttachment.setInitialLayout(vk::ImageLayout::eUndefined); // �J�n���̃��C�A�E�g
    colorAttachment.setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal); // �I�����̃��C�A�E�g

    vk::AttachmentDescription depthAttachment = {};
    depthAttachment.setFormat(chooseSupportedFormat(
        { vk::Format::eD32SfloatS8Uint, vk::Format::eD32Sfloat, vk::Format::eD24UnormS8Uint },
        vk::ImageTiling::eOptimal,
        vk::FormatFeatureFlagBits::eDepthStencilAttachment));
    depthAttachment.setSamples(vk::SampleCountFlagBits::e1);
    depthAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
    depthAttachment.setStoreOp(vk::AttachmentStoreOp::eDontCare);
    depthAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
    depthAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
    depthAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
    depthAttachment.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

    // �J���[�o�b�t�@�̎Q�Ƃ�ݒ�
    vk::AttachmentReference colorAttachmentReference = {};
    colorAttachmentReference.setAttachment(1); // �A�^�b�`�����g�̃C���f�b�N�X
    colorAttachmentReference.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    // �f�v�X�o�b�t�@�̎Q�Ƃ�ݒ�
    vk::AttachmentReference depthAttachmentReference = {};
    depthAttachmentReference.setAttachment(2);
    depthAttachmentReference.setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

    // �T�u�p�X1�̐ݒ�
    vk::SubpassDescription subpass1 = {};
    subpass1.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
    subpass1.setColorAttachmentCount(1);
    subpass1.setPColorAttachments(&colorAttachmentReference); // �J���[�A�^�b�`�����g�̎Q��
    subpass1.setPDepthStencilAttachment(&depthAttachmentReference); // �f�v�X�A�^�b�`�����g�̎Q��

    // �X���b�v�`�F�C���p�̃J���[�o�b�t�@�A�^�b�`�����g
    vk::AttachmentDescription swapchainColorAttachment = {};
    swapchainColorAttachment.setFormat(imageFormat); // �X���b�v�`�F�C���̃t�H�[�}�b�g
    swapchainColorAttachment.setSamples(vk::SampleCountFlagBits::e1);
    swapchainColorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
    swapchainColorAttachment.setStoreOp(vk::AttachmentStoreOp::eStore); // �����_�[�p�X��ɕۑ�
    swapchainColorAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
    swapchainColorAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
    swapchainColorAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
    swapchainColorAttachment.setFinalLayout(vk::ImageLayout::ePresentSrcKHR); // �v���[���g�̂��߂̃��C�A�E�g

    // �X���b�v�`�F�C���̃J���[�o�b�t�@�̎Q�Ƃ�ݒ�
    vk::AttachmentReference swapchainColorAttachmentReference = {};
    swapchainColorAttachmentReference.setAttachment(0);
    swapchainColorAttachmentReference.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    // �T�u�p�X2�̓��̓A�^�b�`�����g�̎Q�Ƃ�ݒ�
    std::array<vk::AttachmentReference, 2> inputReferences = {
        vk::AttachmentReference{1, vk::ImageLayout::eShaderReadOnlyOptimal}, // �T�u�p�X1�̃J���[�A�^�b�`�����g
        vk::AttachmentReference{2, vk::ImageLayout::eShaderReadOnlyOptimal}  // �T�u�p�X1�̃f�v�X�A�^�b�`�����g
    };

    // �T�u�p�X2�̐ݒ�
    vk::SubpassDescription subpass2 = {};
    subpass2.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
    subpass2.setColorAttachmentCount(1);
    subpass2.setPColorAttachments(&swapchainColorAttachmentReference); // �X���b�v�`�F�C���̃J���[�A�^�b�`�����g
    subpass2.setInputAttachmentCount(static_cast<uint32_t>(inputReferences.size()));
    subpass2.setPInputAttachments(inputReferences.data()); // ���̓A�^�b�`�����g�̎Q��

    // �T�u�p�X�ˑ��֌W�̐ݒ�
    std::array<vk::SubpassDependency, 3> subpassDependencies;

    // �O������̐F�A�^�b�`�����g�ւ̃��C�A�E�g�ϊ�
    subpassDependencies[0] = vk::SubpassDependency{
        vk::SubpassExternal,
        0,
        vk::PipelineStageFlagBits::eBottomOfPipe,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::AccessFlagBits::eMemoryRead,
        vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
        {}
    };

    // �T�u�p�X1����T�u�p�X2�ւ̃��C�A�E�g�ϊ�
    subpassDependencies[1] = vk::SubpassDependency{
        0,
        1,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eFragmentShader,
        vk::AccessFlagBits::eColorAttachmentWrite,
        vk::AccessFlagBits::eShaderRead,
        {}
    };

    // �T�u�p�X1����O���ւ̃��C�A�E�g�ϊ�
    subpassDependencies[2] = vk::SubpassDependency{
        0,
        vk::SubpassExternal,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eBottomOfPipe,
        vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
        vk::AccessFlagBits::eMemoryRead,
        {}
    };

    // �����_�[�p�X�̃A�^�b�`�����g��ݒ�
    std::array<vk::AttachmentDescription, 3> renderPassAttachments = { swapchainColorAttachment, colorAttachment, depthAttachment };

    // �����_�[�p�X�쐬���̐ݒ�
    vk::RenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.setAttachmentCount(static_cast<uint32_t>(renderPassAttachments.size()));
    renderPassCreateInfo.setPAttachments(renderPassAttachments.data());
    renderPassCreateInfo.setSubpassCount(static_cast<uint32_t>(2));
    std::array<vk::SubpassDescription, 2> subpasses = { subpass1, subpass2 };
    renderPassCreateInfo.setPSubpasses(subpasses.data());
    renderPassCreateInfo.setDependencyCount(static_cast<uint32_t>(subpassDependencies.size()));
    renderPassCreateInfo.setPDependencies(subpassDependencies.data());

    // �����_�[�p�X�̍쐬
    auto renderPass = logicalDevice.createRenderPass(renderPassCreateInfo);

    // �G���[�`�F�b�N
    if (!renderPass) {
        throw std::runtime_error("Failed to create a Render Pass!"); // �����_�[�p�X�쐬���s��
    }

    return renderPass; // �쐬���������_�[�p�X��Ԃ�
}
