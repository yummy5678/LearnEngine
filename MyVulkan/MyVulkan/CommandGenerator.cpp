#include "CommandGenerator.h"
#include "RenderConfig.h"



SwapChainCommandGenerator::SwapChainCommandGenerator():
    m_LogicalDevice(),
    m_PhysicalDevice(),
    m_CommandPool(),
    m_CommandBuffers()
{
    m_ClassName = "CommandGenerator";
}

SwapChainCommandGenerator::~SwapChainCommandGenerator()
{

}

void SwapChainCommandGenerator::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t commandSize)
{
    m_bCreated = true;

    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;

    // �Z�}�t�H�̍쐬
    m_SemaphoreGenerator.Create(logicalDevice, commandSize);
    m_SignalSemaphores = m_SemaphoreGenerator.GetSignalSemaphores();
    m_WaitSemaphores = m_SemaphoreGenerator.GetWaitSemaphores();

    //�t�F���X�̍쐬
    m_FenceGenerator.Create(logicalDevice, commandSize);
    m_Fences = m_FenceGenerator.GetFence();

    //�R�}���h�v�[��(�R�}���h��u���̈�)���쐬
    m_CommandPool = CreateCommandPool(logicalDevice, physicalDevice);

    //�R�}���h�v�[���ɃR�}���h�o�b�t�@�����蓖��
    m_CommandBuffers = CreateCommandBuffers(logicalDevice, commandSize, m_CommandPool);




}

void SwapChainCommandGenerator::Destroy()
{
    //�R�}���h�v�[���̉��
    m_LogicalDevice.freeCommandBuffers(
        m_CommandPool,
        m_CommandBuffers.size(),
        m_CommandBuffers.data());

    //�R�}���h�v�[���̔j��
    m_LogicalDevice.destroyCommandPool(m_CommandPool);
}


vk::CommandPool SwapChainCommandGenerator::GetCammandPool()
{
    CheckCreated();
    return m_CommandPool;
}

std::vector<vk::CommandBuffer> SwapChainCommandGenerator::GetCommandBuffers()
{
    CheckCreated();
    return m_CommandBuffers;
}

// �_�C�i�~�b�N�����_�����O�ɖ��Ή�
void SwapChainCommandGenerator::DrawFrame(
<<<<<<< HEAD
    vk::CommandBuffer commandBuffer,
    std::vector<RenderConfig>& configs, 
    vk::ImageView colorImage, 
    vk::ImageView depthImage)
=======
    vk::CommandBuffer			commandBuffer,
    //vk::RenderPass				renderpass,
    //vk::Framebuffer				framebuffer,
    vk::Pipeline				graphicsPipeline,
    vk::PipelineLayout			pipelineLayout,
    std::vector<SceneObject>	drawMeshes,
    SceneCamera                 sceneCamera,
    vk::Rect2D					renderArea)
>>>>>>> 011c80f570db61d0cf1756b66acf04ca41bd1a4d
{

    // �J���[�o�b�t�@�A�^�b�`�����g
    vk::RenderingAttachmentInfo colorAttachment;
    colorAttachment.imageView = colorImage;
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.clearValue = vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}));
    
    // Depth�o�b�t�@�A�^�b�`�����g�i3D�I�u�W�F�N�g�p�Ɏg�p�j
    vk::RenderingAttachmentInfo depthAttachment;
    depthAttachment.imageView = depthImage;
    depthAttachment.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.clearValue = vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0));


    for (auto& config : configs)
    {
        // �_�C�i�~�b�N�����_�����O�̐ݒ�
        vk::RenderingInfo renderingInfo;
        renderingInfo.renderArea = config.GetRenderRect();
        renderingInfo.layerCount = 1;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachments = &colorAttachment;
        renderingInfo.pDepthAttachment = &depthAttachment;

        // Dynamic Rendering���J�n
        commandBuffer.beginRendering(renderingInfo);

        // �g�p����p�C�v���C�����o�C���h���܂��B
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, config.GetPipeline());

        Scene* scene = config.GetPScene();
        RenderObjects(commandBuffer, config.GetPipelineLayout(), scene->GetObjects(), scene->GetMainCamera());
        
    }

<<<<<<< HEAD
    commandBuffer.endRendering();
=======
    // �����_�[�p�X�̊J�n����ݒ肵�܂��B
    vk::RenderPassBeginInfo renderpassBeginInfo;
    renderpassBeginInfo
        //.setRenderPass(renderpass)              // �g�p���郌���_�[�p�X
        //.setFramebuffer(framebuffer)            // �g�p����t���[���o�b�t�@
        .setRenderArea(renderArea)              // �`��̈�̐ݒ�
        .setClearValueCount(clearValues.size()) // �N���A�l�̐�
        .setPClearValues(clearValues.data());   // �N���A�l�̔z��
>>>>>>> 011c80f570db61d0cf1756b66acf04ca41bd1a4d

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

void SwapChainCommandGenerator::PresentFrame(vk::SwapchainKHR swapchain)
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

vk::CommandPool SwapChainCommandGenerator::CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
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

std::vector<vk::CommandBuffer> SwapChainCommandGenerator::CreateCommandBuffers(vk::Device logicalDevice, uint32_t commandSize, vk::CommandPool commandPool)
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

vk::SubmitInfo SwapChainCommandGenerator::CreateSubmitInfo(std::vector<vk::CommandBuffer>& commandBuffers, std::vector<vk::Semaphore>& signalSemaphores, std::vector<vk::Semaphore>& waitSemaphores)
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

vk::SubmitInfo SwapChainCommandGenerator::CreateSubmitInfo(vk::CommandBuffer& commandBuffer)
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

uint32_t SwapChainCommandGenerator::AcquireSwapchainNextImage(vk::SwapchainKHR swapchain)
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

void SwapChainCommandGenerator::RenderObjects(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, std::vector<SceneObject> drawMeshes, SceneCamera sceneCamera)
{
    // �`�悷�郁�b�V�������[�v���܂��B
    for (auto& model : drawMeshes)
    {
        // �v�b�V���萔���V�F�[�_�[�ɓn���܂��B
        commandBuffer.pushConstants(
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
            commandBuffer.bindVertexBuffers(0, mesh.GetVertex().GetBuffer(), { 0 });

            // �f�B�X�N���v�^�Z�b�g���o�C���h���܂��B
            std::array<vk::DescriptorSet, 2> descriptorSetGroup =
            {

                //descriptorSets[currentImage], //���Ԃ�J������񂪓����Ă�(uboViewProjection)
                //samplerDescriptorSets[thisModel.getMesh(k)->getTexId()]
                sceneCamera.GetDescriptorSet(),
                model.GetMaterials()[0].GetDescriptorSet()
            };

            commandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                pipelineLayout,
                0,
                descriptorSetGroup,
                nullptr
            );

            // �C���f�b�N�X�o�b�t�@(���_�����ԏ��Ԃ̒l)�����ѕt���܂��B
            commandBuffer.bindIndexBuffer(mesh.GetIndex().GetBuffer(), 0, vk::IndexType::eUint32);
            commandBuffer.drawIndexed(mesh.GetIndex().GetSize(), 1, 0, 0, 0);   // �C���f�b�N�X�ɏ]���ĕ`��
        }
}



