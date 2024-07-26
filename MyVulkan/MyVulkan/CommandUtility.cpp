#include "CommandUtility.h"


/// <summary>
/// �R�}���h�v�[���̍쐬
/// </summary>
vk::UniqueCommandPool CommandUtility::createCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	// �f�o�C�X����L���[�t�@�~���[�̃C���f�b�N�X���擾����
	QueueFamilyIndices queueFamilyIndices = VulkanUtility::GetQueueFamilies(physicalDevice, surface);

	// �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
	vk::CommandPoolCreateInfo poolInfo = {};
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;			// ���̃R�}���h�v�[�����g�p����L���[�t�@�~���[�^�C�v
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;	// �R�}���h�o�b�t�@�̃��Z�b�g��������ꍇ�̓t���O��ǉ�����

	// �O���t�B�b�N�X�L���[�t�@�~���[�p�̃R�}���h�v�[�����쐬����
	vk::CommandPool commandPool = logicalDevice.createCommandPool(poolInfo);
	if (!commandPool)
	{
		throw std::runtime_error("�R�}���h�v�[���̍쐬�Ɏ��s���܂����I");
	}

	return vk::UniqueCommandPool(commandPool,logicalDevice);
}

/// <summary>
/// �R�}���h�o�b�t�@�̍쐬
/// </summary>
std::vector<vk::CommandBuffer> CommandUtility::createCommandBuffers(vk::Device logicalDevice, std::vector<vk::Framebuffer> framebuffers, vk::CommandPool commandPool)
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

    // �G���[�`�F�b�N: framebuffers����łȂ����Ƃ��m�F
    if (framebuffers.empty())
    {
        throw std::runtime_error("framebuffers����ł��I");
    }

    std::vector<vk::CommandBuffer> commandBuffers;
    commandBuffers.reserve(framebuffers.size()); // �e�ʂ��m��

    // �R�}���h�o�b�t�@�����蓖�Ă邽�߂̏���ݒ肷��
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = commandPool;                                 // �R�}���h�o�b�t�@�����蓖�Ă�R�}���h�v�[��
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;                  // �R�}���h�o�b�t�@�̃��x�� (PRIMARY: ���ڃL���[�ɑ��M����o�b�t�@)
    cbAllocInfo.commandBufferCount = (uint32_t)framebuffers.size(); // ���蓖�Ă�R�}���h�o�b�t�@�̐�

    // �R�}���h�o�b�t�@�����蓖�ĂāA���̃n���h�����o�b�t�@�̔z��Ɋi�[����
    commandBuffers = logicalDevice.allocateCommandBuffers(cbAllocInfo);

    // �G���[�`�F�b�N: �R�}���h�o�b�t�@�̊��蓖�ĂɎ��s���Ă��Ȃ����Ƃ��m�F
    if (commandBuffers.empty())
    {
        throw std::runtime_error("�R�}���h�o�b�t�@�̊��蓖�ĂɎ��s���܂����I");
    }

    return commandBuffers;
}

void CommandUtility::recordCommands(vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline, std::vector<vk::Framebuffer> framebuffers, std::vector<vk::CommandBuffer> commandBuffers)
{
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

    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
        renderPassBeginInfo.framebuffer = framebuffers[i];          // �g�p����t���[���o�b�t�@��ݒ肷��

        // �R�}���h�o�b�t�@�̋L�^���J�n����
        vk::Result result = commandBuffers[i].begin(&bufferBeginInfo);
        if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error("�R�}���h�o�b�t�@�̋L�^�̊J�n�Ɏ��s���܂����I");
        }

        // �����_�[�p�X���J�n����
        commandBuffers[i].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        // �g�p����p�C�v���C�����o�C���h����
        commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

        // �p�C�v���C�������s����
        commandBuffers[i].draw(3, 1, 0, 0);

        // �����_�[�p�X���I������
        commandBuffers[i].endRenderPass();

        // �R�}���h�o�b�t�@�̋L�^���I������
        //result = commandBuffers[i].end();
        commandBuffers[i].end();
        //if (result != vk::Result::eSuccess)
        //{
        //    throw std::runtime_error("�R�}���h�o�b�t�@�̋L�^�̏I���Ɏ��s���܂����I");
        //}
    }
}

void CommandUtility::createSynchronisation()
{
    //// MAX_FRAME_DRAWS ���̃Z�}�t�H�ƃt�F���X�����T�C�Y����
    //imageAvailablek.resize(MAX_FRAME_DRAWS);
    //renderFinished.resize(MAX_FRAME_DRAWS);
    //drawFences.resize(MAX_FRAME_DRAWS);

    //// �Z�}�t�H�̍쐬����ݒ肷��
    //VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    //semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    //// �t�F���X�̍쐬����ݒ肷��
    //VkFenceCreateInfo fenceCreateInfo = {};
    //fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    //fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // ������ԂŃV�O�i�����ꂽ��Ԃ̃t�F���X���쐬����

    //// MAX_FRAME_DRAWS ���̃Z�}�t�H�ƃt�F���X���쐬���郋�[�v
    //for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
    //{
    //    // �Z�}�t�H�ƃt�F���X���쐬����
    //    if (vkCreateSemaphore(logicalDevice.get(), &semaphoreCreateInfo, nullptr, &imageAvailable[i]) != VK_SUCCESS ||
    //        vkCreateSemaphore(logicalDevice.get(), &semaphoreCreateInfo, nullptr, &renderFinished[i]) != VK_SUCCESS ||
    //        vkCreateFence(logicalDevice.get(), &fenceCreateInfo, nullptr, &drawFences[i]) != VK_SUCCESS)
    //    {
    //        // �쐬�Ɏ��s�����ꍇ�͗�O�𓊂���
    //        throw std::runtime_error("Failed to create a Semaphore and/or Fence!");
    //    }
    //}
}

CommandGenerator::CommandGenerator()
{
    m_ClassName = "CommandGenerator";
}

CommandGenerator::~CommandGenerator()
{
}

void CommandGenerator::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, std::vector<vk::Framebuffer> framebuffers)
{
    m_bCreated = true;
    m_Framebuffers = framebuffers;
    m_Pool = CreateCommandPool(logicalDevice, physicalDevice, surface);
    m_Buffers = CreateCommandBuffers(logicalDevice, framebuffers, m_Pool);


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

    for (size_t i = 0; i < m_Buffers.size(); i++)
    {
        renderPassBeginInfo.framebuffer = m_Framebuffers[i];          // �g�p����t���[���o�b�t�@��ݒ肷��

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

vk::CommandPool CommandGenerator::CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    // �f�o�C�X����L���[�t�@�~���[�̃C���f�b�N�X���擾����
    QueueFamilyIndices queueFamilyIndices = VulkanUtility::GetQueueFamilies(physicalDevice, surface);

    // �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
    vk::CommandPoolCreateInfo poolInfo = {};
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;			// ���̃R�}���h�v�[�����g�p����L���[�t�@�~���[�^�C�v
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;	// �R�}���h�o�b�t�@�̃��Z�b�g��������ꍇ�̓t���O��ǉ�����

    // �O���t�B�b�N�X�L���[�t�@�~���[�p�̃R�}���h�v�[�����쐬����
    return logicalDevice.createCommandPool(poolInfo);
    //if (!commandPool)
    //{
    //    throw std::runtime_error("�R�}���h�v�[���̍쐬�Ɏ��s���܂����I");
    //}
}

std::vector<vk::CommandBuffer> CommandGenerator::CreateCommandBuffers(vk::Device logicalDevice, std::vector<vk::Framebuffer> framebuffers, vk::CommandPool commandPool)
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

    // �G���[�`�F�b�N: framebuffers����łȂ����Ƃ��m�F
    if (framebuffers.empty())
    {
        throw std::runtime_error("framebuffers����ł��I");
    }

    std::vector<vk::CommandBuffer> commandBuffers;
    commandBuffers.reserve(framebuffers.size()); // �e�ʂ��m��

    // �R�}���h�o�b�t�@�����蓖�Ă邽�߂̏���ݒ肷��
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = commandPool;                                 // �R�}���h�o�b�t�@�����蓖�Ă�R�}���h�v�[��
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;                  // �R�}���h�o�b�t�@�̃��x�� (PRIMARY: ���ڃL���[�ɑ��M����o�b�t�@)
    cbAllocInfo.commandBufferCount = (uint32_t)framebuffers.size(); // ���蓖�Ă�R�}���h�o�b�t�@�̐�

    // �R�}���h�o�b�t�@�����蓖�ĂāA���̃n���h�����o�b�t�@�̔z��Ɋi�[����
    commandBuffers = logicalDevice.allocateCommandBuffers(cbAllocInfo);

    // �G���[�`�F�b�N: �R�}���h�o�b�t�@�̊��蓖�ĂɎ��s���Ă��Ȃ����Ƃ��m�F
    if (commandBuffers.empty())
    {
        throw std::runtime_error("�R�}���h�o�b�t�@�̊��蓖�ĂɎ��s���܂����I");
    }

    return commandBuffers;
}
