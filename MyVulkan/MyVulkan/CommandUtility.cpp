#include "CommandUtility.h"


CommandGenerator::CommandGenerator()
{
    m_ClassName = "CommandGenerator";
}

CommandGenerator::~CommandGenerator()
{

}

void CommandGenerator::Create(vk::Device logicalDevice, vk::PhysicalDevice phygicalDevice, uint32_t commandSize)
{
    m_bCreated = true;

    //�R�}���h�v�[��(�R�}���h��u���̈�)���쐬
    m_CommandPool = CreateCommandPool(logicalDevice, phygicalDevice);

    //�R�}���h�v�[���ɃR�}���h�o�b�t�@�����蓖��
    m_Buffers = CreateCommandBuffers(logicalDevice, commandSize, m_CommandPool);


}

void CommandGenerator::Destroy()
{
    //�R�}���h�v�[���̉��
    m_LogicalDevice.freeCommandBuffers(
        m_CommandPool,
        m_Buffers.size(),
        m_Buffers.data());

    //�R�}���h�v�[���̔j��
    m_LogicalDevice.destroyCommandPool(m_CommandPool);
}

void CommandGenerator::RecordGraphicCommands(std::vector<vk::Framebuffer> framebuffers, vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline)
{
    CheckCreated();

    // �e�R�}���h�o�b�t�@�̊J�n���@�Ɋւ�����
    vk::CommandBufferBeginInfo bufferBeginInfo;
    bufferBeginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse; // �o�b�t�@���Ďg�p�\�ł��邱�Ƃ������t���O

    // �����_�[�p�X���J�n���邽�߂̏�� (�O���t�B�J���ȃA�v���P�[�V�����̏ꍇ�̂ݕK�v)
    vk::RenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.renderPass = renderPass;                             // �J�n���郌���_�[�p�X
    renderPassBeginInfo.renderArea.offset = vk::Offset2D{ 0, 0 };              // �����_�[�p�X�̊J�n�ʒu (�s�N�Z���P��)
    renderPassBeginInfo.renderArea.extent = extent;                          // �����_�[�p�X�����s����̈�̃T�C�Y (offset����n�܂�)
    std::array<vk::ClearValue, 1> clearValues = {
        vk::ClearValue{std::array<float, 4>{0.6f, 0.65f, 0.4f, 1.0f}}        // �N���A����l�̃��X�g
    };
    renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassBeginInfo.pClearValues = clearValues.data();                   // �N���A����l�̃��X�g



    //�Ƃ肠������̃R�}���h���쐬
    for (size_t i = 0; i < m_Buffers.size(); i++)
    {
        renderPassBeginInfo.framebuffer = framebuffers[i];          // �g�p����t���[���o�b�t�@��ݒ肷��

        // �R�}���h�o�b�t�@�̋L�^���J�n����
        vk::Result result = m_Buffers[i].begin(&bufferBeginInfo);
        if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error("�R�}���h�o�b�t�@�̋L�^�̊J�n�Ɏ��s���܂����I");
        }

        // �����_�[�p�X���J�n����
        m_Buffers[i].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        // �g�p����p�C�v���C�����o�C���h����
        m_Buffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

        // �p�C�v���C�������s����
        m_Buffers[i].draw(3, 1, 0, 0);

        // �����_�[�p�X���I������
        m_Buffers[i].endRenderPass();

        // �R�}���h�o�b�t�@�̋L�^���I������
        //result = m_Buffers[i].end();
        m_Buffers[i].end();
        if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error("�R�}���h�o�b�t�@�̋L�^�̏I���Ɏ��s���܂����I");
        }
    }
}

//void CommandGenerator::RecordGraphicCommands(vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline)
//{
//    CheckCreated();
//
//    // �e�R�}���h�o�b�t�@�̊J�n���@�Ɋւ�����
//    vk::CommandBufferBeginInfo bufferBeginInfo;
//    bufferBeginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse; // �o�b�t�@���Ďg�p�\�ł��邱�Ƃ������t���O
//
//    // �����_�[�p�X���J�n���邽�߂̏�� (�O���t�B�J���ȃA�v���P�[�V�����̏ꍇ�̂ݕK�v)
//    vk::RenderPassBeginInfo renderPassBeginInfo;
//    renderPassBeginInfo.renderPass = renderPass;                             // �J�n���郌���_�[�p�X
//    renderPassBeginInfo.renderArea.offset = vk::Offset2D{ 0, 0 };              // �����_�[�p�X�̊J�n�ʒu (�s�N�Z���P��)
//    renderPassBeginInfo.renderArea.extent = extent;                          // �����_�[�p�X�����s����̈�̃T�C�Y (offset����n�܂�)
//    std::array<vk::ClearValue, 1> clearValues = {
//        vk::ClearValue{std::array<float, 4>{0.6f, 0.65f, 0.4f, 1.0f}}        // �N���A����l�̃��X�g
//    };
//    renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
//    renderPassBeginInfo.pClearValues = clearValues.data();                   // �N���A����l�̃��X�g
//
//
//
//    //�Ƃ肠������̃R�}���h���쐬
//    for (size_t i = 0; i < m_Buffers.size(); i++)
//    {
//        //renderPassBeginInfo.framebuffer = m_Framebuffers[i];          // �g�p����t���[���o�b�t�@��ݒ肷��
//
//        // �R�}���h�o�b�t�@�̋L�^���J�n����
//        vk::Result result = m_Buffers[i].begin(&bufferBeginInfo);
//        if (result != vk::Result::eSuccess)
//        {
//            throw std::runtime_error("�R�}���h�o�b�t�@�̋L�^�̊J�n�Ɏ��s���܂����I");
//        }
//
//        // �����_�[�p�X���J�n����
//        m_Buffers[i].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);
//
//        // �g�p����p�C�v���C�����o�C���h����
//        m_Buffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
//
//        // �p�C�v���C�������s����
//        m_Buffers[i].draw(3, 1, 0, 0);
//
//        // �����_�[�p�X���I������
//        m_Buffers[i].endRenderPass();
//
//        // �R�}���h�o�b�t�@�̋L�^���I������
//        //result = m_Buffers[i].end();
//        m_Buffers[i].end();
//        if (result != vk::Result::eSuccess)
//        {
//            throw std::runtime_error("�R�}���h�o�b�t�@�̋L�^�̏I���Ɏ��s���܂����I");
//        }
//    }
//}

vk::CommandPool CommandGenerator::GetCammandPool()
{
    CheckCreated();
    return m_CommandPool;
}

std::vector<vk::CommandBuffer> CommandGenerator::GetCommandBuffers()
{
    CheckCreated();
    return m_Buffers;
}

vk::CommandPool CommandGenerator::CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice phygicalDevice)
{
    QueueFamilySelector queue(phygicalDevice);
    // �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.pNext;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;    // �R�}���h�o�b�t�@�̃��Z�b�g��������ꍇ�̓t���O��ǉ�����
    poolInfo.queueFamilyIndex = queue.GetGraphicIndex();	            // ���̃R�}���h�v�[�����g�p����L���[�t�@�~���[�^�C�v
    poolInfo.allowDuplicate;

    // �O���t�B�b�N�X�L���[�t�@�~���[�p�̃R�}���h�v�[�����쐬����
    return logicalDevice.createCommandPool(poolInfo);
    //if (!commandPool)
    //{
    //    throw std::runtime_error("�R�}���h�v�[���̍쐬�Ɏ��s���܂����I");
    //}
}

std::vector<vk::CommandBuffer> CommandGenerator::CreateCommandBuffers(vk::Device logicalDevice, uint32_t commandSize, vk::CommandPool commandPool)
{

    // �R�}���h�o�b�t�@���A���P�[�g(���蓖�Ă�)���߂̏���ݒ肷��
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = commandPool;                  // �R�}���h�o�b�t�@�����蓖�Ă�R�}���h�v�[��
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;   // �R�}���h�o�b�t�@�̎��(PRIMARY: ���ڃL���[�ɑ��M����o�b�t�@)
    cbAllocInfo.commandBufferCount = commandSize;           // ���蓖�Ă�R�}���h�o�b�t�@�̐�

    // �R�}���h�o�b�t�@�����蓖�ĂāA���̃n���h�����o�b�t�@�̔z��Ɋi�[����
    std::vector<vk::CommandBuffer> commandBuffers = logicalDevice.allocateCommandBuffers(cbAllocInfo); //�z��ŏ�������肷��

    // �G���[�`�F�b�N
    if (commandBuffers.empty())
    {
        throw std::runtime_error("�R�}���h�o�b�t�@�̊��蓖�ĂɎ��s���܂����I");
    }

    return commandBuffers;
}



