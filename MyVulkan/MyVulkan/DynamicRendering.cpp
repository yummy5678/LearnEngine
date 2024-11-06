#include "DynamicRendering.h"

VDynamicRenderingCommand::VDynamicRenderingCommand(DeviceExtensionManager& deviceExtensionManager)
{
	deviceExtensionManager.UseDynamicRendering();
}

VDynamicRenderingCommand::~VDynamicRenderingCommand()
{
}

void VDynamicRenderingCommand::RenderWithDynamicRendering(
    vk::ImageView               colorImageView,
    vk::Extent2D                extent, 
    vk::Pipeline                graphicsPipeline, 
    vk::PipelineLayout          pipelineLayout,
    std::vector<SceneObject>    drawModels, 
    SceneCamera                 sceneCamera)
{
    // 1. �J���[�A�^�b�`�����g���̐ݒ�
    vk::ClearValue clearColor = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}); // ���F�ŃN���A

    vk::RenderingAttachmentInfo colorAttachmentInfo;
    colorAttachmentInfo
        .setImageView(colorImageView)                                // �J���[�C���[�W���w��
        .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)    // ���C�A�E�g���œK��
        .setLoadOp(vk::AttachmentLoadOp::eClear)                     // �`��O�ɃN���A
        .setStoreOp(vk::AttachmentStoreOp::eStore)                   // �`���Ɍ��ʂ�ێ�
        .setClearValue(clearColor);                                  // �N���A�J���[�ݒ�

    // 2. �����_�����O���̐ݒ�
    vk::RenderingInfo renderingInfo;
    
    renderingInfo.setRenderArea(vk::Rect2D({ 0, 0 }, extent));  // �`��G���A
    renderingInfo.setLayerCount(1);                             // ���C���[��
    renderingInfo.setColorAttachments(colorAttachmentInfo);     // �J���[�A�^�b�`�����g�����w��

    // 3. Dynamic Rendering�̊J�n
    m_CommandBuffer.beginRendering(renderingInfo);

    // �����ŃO���t�B�b�N�X�p�C�v���C�����o�C���h���ĕ`����s���܂�
    m_CommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

    // �`�悷�郁�b�V�������[�v���܂��B
    for (auto& model : drawModels)
    {
        // �v�b�V���萔���V�F�[�_�[�ɓn���܂��B
        m_CommandBuffer.pushConstants(
            pipelineLayout,
            vk::ShaderStageFlagBits::eVertex,   // �v�b�V���萔���X�V����V�F�[�_�[�X�e�[�W
            0,                                  // �I�t�Z�b�g
            sizeof(Transform),                  // �v�b�V������f�[�^�̃T�C�Y
            &model.GetTransform()               // ���ۂ̃f�[�^
        );

        // �e���b�V�������[�v���܂��B
        for (auto& mesh : model.GetMeshes())
        {
            // ���_�o�b�t�@���o�C���h
            m_CommandBuffer.bindVertexBuffers(0, mesh.GetVertex().GetBuffer(), { 0 });

            // �f�B�X�N���v�^�Z�b�g���o�C���h���܂��B
            std::array<vk::DescriptorSet, 2> descriptorSetGroup =
            {

                //descriptorSets[currentImage], //���Ԃ�J������񂪓����Ă�(uboViewProjection)
                //samplerDescriptorSets[thisModel.getMesh(k)->getTexId()]
                sceneCamera.GetDescriptorSet(),             // 
                model.GetMaterials()[0].GetDescriptorSet()  // �e�N�X�`���摜�̃f�B�X�N�v���^
            };

            m_CommandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                pipelineLayout,
                0,
                descriptorSetGroup,
                nullptr
            );

            // �C���f�b�N�X�o�b�t�@(���_�����ԏ��Ԃ̒l)�����ѕt���܂��B
            m_CommandBuffer.bindIndexBuffer(mesh.GetIndex().GetBuffer(), 0, vk::IndexType::eUint32);
            m_CommandBuffer.drawIndexed(mesh.GetIndex().GetSize(), 1, 0, 0, 0);   // �C���f�b�N�X�ɏ]���ĕ`��
        }


    // 4. Dynamic Rendering�̏I��
    m_CommandBuffer.endRendering();
}
