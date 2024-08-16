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
    m_Pool = CreateCommandPool(logicalDevice, phygicalDevice);
    m_Buffers = CreateCommandBuffers(logicalDevice, commandSize, m_Pool);
}

void CommandGenerator::Destroy(vk::Device logicalDevice)
{
    vkDestroyCommandPool(logicalDevice, m_Pool, nullptr);
}

void CommandGenerator::RecordCommands(vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline)
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
        //renderPassBeginInfo.framebuffer = m_Framebuffers[i];          // �g�p����t���[���o�b�t�@��ݒ肷��

        // �R�}���h�o�b�t�@�̋L�^���J�n����
        vk::Result result = m_Buffers[i].begin(&bufferBeginInfo);
        if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error("�R�}���h�o�b�t�@�̋L�^�̊J�n�Ɏ��s���܂����I");
        }

        // �����_�[�p�X���J�n����
        m_Buffers[i].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        //// �g�p����p�C�v���C�����o�C���h����
        //m_Buffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

        //// �p�C�v���C�������s����
        //m_Buffers[i].draw(3, 1, 0, 0);

        //// �����_�[�p�X���I������
        //m_Buffers[i].endRenderPass();

        // �R�}���h�o�b�t�@�̋L�^���I������
        //result = commandBuffers[i].end();
        m_Buffers[i].end();
        //if (result != vk::Result::eSuccess)
        //{
        //    throw std::runtime_error("�R�}���h�o�b�t�@�̋L�^�̏I���Ɏ��s���܂����I");
        //}
    }
}

vk::CommandPool CommandGenerator::GetPool()
{
    CheckCreated();
    return m_Pool;
}

std::vector<vk::CommandBuffer> CommandGenerator::GetBuffers()
{
    CheckCreated();
    return m_Buffers;
}

vk::CommandPool CommandGenerator::CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice phygicalDevice)
{
    QueueFamilySelector queue(phygicalDevice);
    // �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.queueFamilyIndex = queue.GetGraphicIndex();	            // ���̃R�}���h�v�[�����g�p����L���[�t�@�~���[�^�C�v
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;    // �R�}���h�o�b�t�@�̃��Z�b�g��������ꍇ�̓t���O��ǉ�����

    // �O���t�B�b�N�X�L���[�t�@�~���[�p�̃R�}���h�v�[�����쐬����
    return logicalDevice.createCommandPool(poolInfo);
    //if (!commandPool)
    //{
    //    throw std::runtime_error("�R�}���h�v�[���̍쐬�Ɏ��s���܂����I");
    //}
}

std::vector<vk::CommandBuffer> CommandGenerator::CreateCommandBuffers(vk::Device logicalDevice, uint32_t commandSize, vk::CommandPool commandPool)
{
    // �G���[�`�F�b�N: logicalDevice���L���ł��邩�m�F
    if (!logicalDevice)
    {
        throw std::runtime_error("logicalDevice�������ł��I");
    }

    // �G���[�`�F�b�N: commandPool���L���ł��邩�m�F
    if (!commandPool)
    {
        throw std::runtime_error("commandPool�������ł��I");
    }

    std::vector<vk::CommandBuffer> commandBuffers;
    commandBuffers.reserve(commandSize); // �e�ʂ��m��

    // �R�}���h�o�b�t�@�����蓖�Ă邽�߂̏���ݒ肷��
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = commandPool;                          // �R�}���h�o�b�t�@�����蓖�Ă�R�}���h�v�[��
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;           // �R�}���h�o�b�t�@�̃��x�� (PRIMARY: ���ڃL���[�ɑ��M����o�b�t�@)
    cbAllocInfo.commandBufferCount = commandSize; // ���蓖�Ă�R�}���h�o�b�t�@�̐�

    // �R�}���h�o�b�t�@�����蓖�ĂāA���̃n���h�����o�b�t�@�̔z��Ɋi�[����
    commandBuffers = logicalDevice.allocateCommandBuffers(cbAllocInfo);

    // �G���[�`�F�b�N: �R�}���h�o�b�t�@�̊��蓖�ĂɎ��s���Ă��Ȃ����Ƃ��m�F
    if (commandBuffers.empty())
    {
        throw std::runtime_error("�R�}���h�o�b�t�@�̊��蓖�ĂɎ��s���܂����I");
    }

    return commandBuffers;
}
