#include "CommandUtility.h"




CommandGenerator::CommandGenerator():
    m_LogicalDevice(),
    m_PhysicalDevice(),
    m_CommandPool(),
    m_CommandBuffers()
{
    m_ClassName = "CommandGenerator";
}

CommandGenerator::~CommandGenerator()
{

}

void CommandGenerator::LoadShader(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t commandSize)
{
    m_bCreated = true;

    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;

    // �Z�}�t�H�̍쐬
    m_SemaphoreGenerator.LoadShader(logicalDevice, commandSize);
    m_SignalSemaphores = m_SemaphoreGenerator.GetSignalSemaphores();
    m_WaitSemaphores = m_SemaphoreGenerator.GetWaitSemaphores();

    //�t�F���X�̍쐬
    m_FenceGenerator.LoadShader(logicalDevice, commandSize);
    m_Fences = m_FenceGenerator.GetFence();

    //�R�}���h�v�[��(�R�}���h��u���̈�)���쐬
    m_CommandPool = CreateCommandPool(logicalDevice, physicalDevice);

    //�R�}���h�v�[���ɃR�}���h�o�b�t�@�����蓖��
    m_CommandBuffers = CreateCommandBuffers(logicalDevice, commandSize, m_CommandPool);




}

void CommandGenerator::Destroy()
{
    //�R�}���h�v�[���̉��
    m_LogicalDevice.freeCommandBuffers(
        m_CommandPool,
        m_CommandBuffers.size(),
        m_CommandBuffers.data());

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


    for (size_t i = 0; i < m_CommandBuffers.size(); i++)
    {
        renderPassBeginInfo.framebuffer = framebuffers[i];          // �g�p����t���[���o�b�t�@��ݒ肷��

        // �R�}���h�o�b�t�@�̋L�^���J�n����
        vk::Result result = m_CommandBuffers[i].begin(&bufferBeginInfo);
        if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error("�R�}���h�o�b�t�@�̋L�^�̊J�n�Ɏ��s���܂����I");
        }

        // �����_�[�p�X���J�n����
        m_CommandBuffers[i].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        // �g�p����p�C�v���C�����o�C���h����
        m_CommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

        // �p�C�v���C�������s����
        m_CommandBuffers[i].draw(3, 1, 0, 0);

        // �����_�[�p�X���I������
        m_CommandBuffers[i].endRenderPass();

        // �R�}���h�o�b�t�@�̋L�^���I������
        //result = m_Buffers[i].end();
        m_CommandBuffers[i].end();
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
    return m_CommandBuffers;
}

void CommandGenerator::DrawFrame(vk::CommandBuffer buffer, vk::RenderPass renderpass, vk::Framebuffer framebuffer, vk::Rect2D renderArea, vk::Pipeline graphicsPipeline, std::vector<SceneObject> drawMeshes)
{
    //�w�肵���t���[���o�b�t�@�Ƀ����_�[�p�X�ƃp�C�v���C�����֘A�t����
    //�������ރR�}���h�̍쐬�Ƒ��M���s���֐�

    //�t���[���̏���������F
    std::array<vk::ClearValue, 3> clearValues = {};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].color = { 0.6f, 0.65f, 0.4f, 1.0f };
    clearValues[2].depthStencil.depth = 1.0f;

    vk::CommandBufferBeginInfo cmdBeginInfo;
    if (buffer.begin(&cmdBeginInfo) != vk::Result::eSuccess)
    {
        throw std::runtime_error("�R�}���h�o�b�t�@�̊J�n�Ɏ��s���܂����I");
    }

    vk::RenderPassBeginInfo renderpassBeginInfo;
    renderpassBeginInfo.renderPass = renderpass;
    renderpassBeginInfo.framebuffer = framebuffer;
    renderpassBeginInfo.renderArea = renderArea;
    renderpassBeginInfo.clearValueCount = clearValues.size();
    renderpassBeginInfo.pClearValues = clearValues.data();

    buffer.beginRenderPass(renderpassBeginInfo, vk::SubpassContents::eInline);

    // �g�p����p�C�v���C�����o�C���h����
    buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

    {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (size_t j = 0; j < drawMeshes.size(); j++)
        {
            auto thisModel = drawMeshes[j];
            auto thisMeshes = thisModel.GetMesh();

            vkCmdPushConstants(
                buffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT,		// Stage to push constants to
                0,								// Offset of push constants to update
                sizeof(Transform),				// Size of data being pushed
                &thisModel.GetTransform());			// Actual data being pushed (can be array)

            for (size_t k = 0; k < thisMeshes.meshes.size(); k++)
            {

                VkBuffer vertexBuffers[] = { thisModel.GetMeshBuffer()};					// Buffers to bind
                VkDeviceSize offsets[] = { 0 };												// Offsets into buffers being bound
                vkCmdBindVertexBuffers(commandBuffers[currentImage], 0, 1, vertexBuffers, offsets);	// Command to bind vertex buffer before drawing with them

                // Bind mesh index buffer, with 0 offset and using the uint32 type
                vkCmdBindIndexBuffer(commandBuffers[currentImage], thisModel.getMesh(k)->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

                // Dynamic Offset Amount
                // uint32_t dynamicOffset = static_cast<uint32_t>(modelUniformAlignment) * j;

                // "Push" constants to given shader stage directly (no buffer)


                std::array<VkDescriptorSet, 2> descriptorSetGroup = { descriptorSets[currentImage],
                    samplerDescriptorSets[thisModel.getMesh(k)->getTexId()] };

                // Bind Descriptor Sets
                vkCmdBindDescriptorSets(commandBuffers[currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
                    0, static_cast<uint32_t>(descriptorSetGroup.size()), descriptorSetGroup.data(), 0, nullptr);

                // Execute pipeline
                vkCmdDrawIndexed(commandBuffers[currentImage], thisModel.getMesh(k)->getIndexCount(), 1, 0, 0, 0);
            }
        }

    }////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // �����ŃT�u�p�X0�Ԃ̏���
    buffer.draw(3, 1, 0, 0);

    buffer.endRenderPass();

    buffer.end();

    auto submit = CreateSubmitInfo(buffer);

    // �g�p����L���[�i�O���t�B�b�N�L���[��v���[���g�L���[�Ȃǁj�̃C���f�b�N�X���擾
    auto queueFamily = QueueFamilySelector(m_PhysicalDevice);
    auto graphicsQueue = m_LogicalDevice.getQueue(queueFamily.GetGraphicIndex(), 0);

    graphicsQueue.submit(submit, nullptr);

    // �I���ҋ@
    //graphicsQueue.waitIdle();

    //vk::PresentInfoKHR presentInfo;
    //presentInfo.pNext;
    //presentInfo.waitSemaphoreCount;
    //presentInfo.pWaitSemaphores = nullptr;
    //presentInfo.swapchainCount;
    //presentInfo.pSwapchains = nullptr;
    //presentInfo.pImageIndices = &imageIndex;
    //presentInfo.pResults;
    //graphicsQueue.presentKHR(presentInfo);
}

void CommandGenerator::PresentFrame(vk::SwapchainKHR swapchain)
{
    vk::PresentInfoKHR presentInfo;

    auto index = AcquireSwapchainNextImage(swapchain);

    auto presentSwapchains = { swapchain };
    auto imgIndices = { index };

    presentInfo.swapchainCount = presentSwapchains.size();
    presentInfo.pSwapchains = presentSwapchains.begin();
    presentInfo.pImageIndices = imgIndices.begin();

    // �g�p����L���[(�O���t�B�b�N�L���[��v���[���g�L���[�Ȃ�)�̃C���f�b�N�X���擾
    auto queueFamily = QueueFamilySelector(m_PhysicalDevice);
    auto graphicsQueue = m_LogicalDevice.getQueue(queueFamily.GetGraphicIndex(), 0);

    if (graphicsQueue.presentKHR(presentInfo) != vk::Result::eSuccess)
    {
        throw std::runtime_error("�X���b�v�`�F�[���̉摜�̕\���Ɏ��s���܂����I");
    }

    // graphicsQueue.waitIdle();
}

vk::CommandPool CommandGenerator::CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
{ 
    // Get indices of queue families from device
    QueueFamilySelector queue(physicalDevice);

    // �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;	// �R�}���h�o�b�t�@�̃��Z�b�g��������ꍇ�̓t���O��ǉ�����
    poolInfo.queueFamilyIndex = queue.GetGraphicIndex();	            // ���̃R�}���h�v�[�����g�p����L���[�t�@�~���[

    // �O���t�B�b�N�X�L���[�t�@�~���[�p�̃R�}���h�v�[�����쐬����
    vk::CommandPool pool;
    try
    {
        pool = logicalDevice.createCommandPool(poolInfo);
    }
    catch (const std::exception&)
    {
        throw std::runtime_error("�R�}���h�v�[���̍쐬�Ɏ��s���܂����I");
    }

    return pool;
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

vk::SubmitInfo CommandGenerator::CreateSubmitInfo(std::vector<vk::CommandBuffer>& commandBuffers, std::vector<vk::Semaphore>& signalSemaphores, std::vector<vk::Semaphore>& waitSemaphores)
{
    vk::SubmitInfo submitInfo;
    //submitInfo.pNext;
    submitInfo.signalSemaphoreCount = m_SignalSemaphores.size();
    submitInfo.pSignalSemaphores = m_SignalSemaphores.data();
    submitInfo.allowDuplicate;
    submitInfo.pWaitDstStageMask;
    submitInfo.waitSemaphoreCount = m_WaitSemaphores.size();
    submitInfo.pWaitSemaphores = m_WaitSemaphores.data();
    submitInfo.commandBufferCount = commandBuffers.size();
    submitInfo.pCommandBuffers = commandBuffers.data();

    return submitInfo;
}

vk::SubmitInfo CommandGenerator::CreateSubmitInfo(vk::CommandBuffer& commandBuffer)
{
    vk::SubmitInfo submitInfo;
    //submitInfo.pNext;
    //submitInfo.signalSemaphoreCount;
    //submitInfo.pSignalSemaphores;
    //submitInfo.allowDuplicate;
    //submitInfo.pWaitDstStageMask;
    //submitInfo.waitSemaphoreCount;
    //submitInfo.pWaitSemaphores;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    return submitInfo;
}

uint32_t CommandGenerator::AcquireSwapchainNextImage(vk::SwapchainKHR swapchain)
{
    // �X���b�v�`�F�[�����玟�ɕ`�悷��C���[�W�i�t���[���o�b�t�@�̂悤�Ȃ��́j�̃C���f�b�N�X���擾���܂��B
    uint32_t imageIndex;
    vk::Result result = m_LogicalDevice.acquireNextImageKHR(
	    swapchain,                              // �X���b�v�`�F�[��
	    std::numeric_limits<uint64_t>::max(),   // �^�C���A�E�g�̐ݒ�i�����ł͖����ҋ@�j
	    //imageAvailable[currentFrame],         // �C���[�W���g�p�\�ɂȂ�̂�ʒm����Z�}�t�H
	    nullptr,                                // �C���[�W���g�p�\�ɂȂ�̂�ʒm����Z�}�t�H
	    nullptr,                                // �t�F���X�i�����ł͎g�p���Ȃ��̂�nullptr�j
	    &imageIndex                             // �C���[�W�̃C���f�b�N�X���i�[�����
    );
    // �C���[�W�擾�Ɏ��s�����ꍇ�A�G���[���b�Z�[�W�𓊂���
    if (result != vk::Result::eSuccess)
    {
	    throw std::runtime_error("�X���b�v�`�F�[������C���[�W���擾�ł��܂���ł����I");
    }

    return imageIndex;
}



