#include "DrawCommand.h"



DrawCommand::DrawCommand() : 
    m_LogicalDevice(VK_NULL_HANDLE),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Swapchain(VK_NULL_HANDLE),
    m_CurrentIndex(0),
    m_NextIndex(0),
    m_ImageSet(),
    m_CommandBuffers(),
    m_CommandPool(VK_NULL_HANDLE),
    m_SignalSemaphores(),
    m_Fences(),
    m_QueueSelector()
{

}

DrawCommand::~DrawCommand()
{
}

void DrawCommand::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, RenderingImageSet* imageSet)
{
    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;
    m_ImageSet = { imageSet };
    m_QueueSelector.Initialize(m_PhysicalDevice);

    // �g�p����t���[���̖���
    size_t imageSize = m_ImageSet.size();

    // �t���[���̐������Z�}�t�H�ƃt�F���X���쐬
    m_SignalSemaphores.resize(imageSize);
    m_ImageAvailableSemaphores.resize(imageSize);
    m_Fences.resize(imageSize);

    //�t�F���X�̍쐬
    CreateFence(m_Fences[0]);

    //�R�}���h�v�[��(�R�}���h��u���̈�)���쐬
    CreateCommandPool();

    //�R�}���h�v�[���ɃR�}���h�o�b�t�@�����蓖��
    CreateCommandBuffers(m_ImageSet.size(), m_CommandPool);

}

void DrawCommand::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, SwapchainRenderer* swapchainRenderer)
{
    m_Swapchain = swapchainRenderer->GetSwapchain();

    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;
    m_ImageSet = swapchainRenderer->GetImageSets();
    m_QueueSelector.Initialize(m_PhysicalDevice);

    // �g�p����t���[���̖���
    size_t imageSize = m_ImageSet.size();

    // �t���[���̐������Z�}�t�H�ƃt�F���X���쐬
    m_SignalSemaphores.resize(imageSize);
    m_ImageAvailableSemaphores.resize(imageSize);
    m_Fences.resize(imageSize);
    for (size_t i = 0; i < imageSize; i++)
    {
        // �Z�}�t�H�̍쐬
        CreateSemaphore(m_SignalSemaphores[i]);
        CreateSemaphore(m_ImageAvailableSemaphores[i]);

        //�t�F���X�̍쐬
        CreateFence(m_Fences[i]);
    }

    //�R�}���h�v�[��(�R�}���h��u���̈�)���쐬
    CreateCommandPool();

    //�R�}���h�v�[���ɃR�}���h�o�b�t�@�����蓖��
    CreateCommandBuffers(imageSize, m_CommandPool);
}

void DrawCommand::Destroy()
{
    //�R�}���h�v�[���̉��
    m_LogicalDevice.freeCommandBuffers(
        m_CommandPool,
        (uint32_t)m_CommandBuffers.size(),
        m_CommandBuffers.data());

    //�R�}���h�v�[���̔j��
    m_LogicalDevice.destroyCommandPool(m_CommandPool);
}

vk::CommandBuffer DrawCommand::GetBuffer()
{
    if (m_CommandBuffers.empty() == true) 
        throw std::runtime_error("�R�}���h�����o�����Ƃ��܂������A����͂܂����s����Ă��܂���");

    return m_CommandBuffers[m_CurrentIndex];
}

void DrawCommand::BeginRendering(vk::Rect2D renderArea)
{
    if (m_ImageSet.empty() == true) return; // �`�悷��ׂ̃C���[�W���Z�b�g����Ă��Ȃ���Ή������Ȃ�

    m_NextIndex = AcquireSwapchainNextImage();

    // �J���[�o�b�t�@�A�^�b�`�����g
    vk::RenderingAttachmentInfo colorAttachment;
    colorAttachment.imageView = m_ImageSet[m_NextIndex].color.imageView;
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.clearValue = vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}));

    // Depth�o�b�t�@�A�^�b�`�����g�i3D�I�u�W�F�N�g�p�Ɏg�p�j
    vk::RenderingAttachmentInfo depthAttachment;
    depthAttachment.imageView = m_ImageSet[m_NextIndex].depth.imageView;
    depthAttachment.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.clearValue = vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0));

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags;
    beginInfo.pNext;
    beginInfo.pInheritanceInfo; // �Z�J���_���R�}���h�o�b�t�@(�g�p���Ȃ�)


    // �_�C�i�~�b�N�����_�����O�̐ݒ�
    vk::RenderingInfo renderingInfo;
    renderingInfo.renderArea = renderArea;
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;
    renderingInfo.pDepthAttachment = &depthAttachment;

    // Dynamic Rendering���J�n
    auto commandBuffer = m_CommandBuffers[m_NextIndex];
    commandBuffer.begin(beginInfo);
    commandBuffer.beginRendering(renderingInfo);



}

void DrawCommand::EndRendering()
{
    m_CommandBuffers[m_NextIndex].endRendering();
    m_CommandBuffers[m_NextIndex].end();

    std::vector<vk::PipelineStageFlags> waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    vk::SubmitInfo submitInfo = CreateSubmitInfo(&waitStages);

    // �L���[�ɃR�}���h�𑗐M
    vk::Queue queue = m_LogicalDevice.getQueue(m_QueueSelector.GetGraphicIndex(), 0);
    queue.submit(submitInfo, m_Fences[m_CurrentIndex]);

    // �C���f�b�N�X�̃J�E���g��i�߂�
    m_CurrentIndex = (m_CurrentIndex + 1) % m_ImageSet.size();   //  �Y�������R�}���h���͈͓̔��Ɏ��܂�悤����
}

vk::Semaphore DrawCommand::GetImageAvableSemaphore()
{
    return m_ImageAvailableSemaphores[m_CurrentIndex];
}

vk::Fence DrawCommand::GetFence()
{
    return m_Fences[m_CurrentIndex];
}

uint32_t DrawCommand::GetCurrentIndex()
{
    return m_CurrentIndex;
}


void DrawCommand::CreateSemaphore(vk::Semaphore& semaphore)
{
    vk::SemaphoreCreateInfo semaphoreInfo;
    semaphoreInfo.pNext;
    semaphore = m_LogicalDevice.createSemaphore(semaphoreInfo);
}

void DrawCommand::CreateFence(vk::Fence& fence)
{
    vk::FenceCreateInfo fenceInfo;
    fenceInfo.pNext;
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
    fence = m_LogicalDevice.createFence(fenceInfo);
}

void DrawCommand::CreateCommandPool()
{
    if(m_LogicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("�R�}���h�v�[�����쐬���悤�Ƃ��܂������A�_���f�o�C�X��NULL�ł�");

    if (m_PhysicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("�R�}���h�v�[�����쐬���悤�Ƃ��܂������A�����f�o�C�X��NULL�ł�");

    // �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.pNext = nullptr;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;	// �R�}���h�o�b�t�@�̃��Z�b�g��������ꍇ�̓t���O��ǉ�����
    poolInfo.queueFamilyIndex = m_QueueSelector.GetGraphicIndex();	            // ���̃R�}���h�v�[�����g�p����L���[�t�@�~���[

   m_CommandPool = m_LogicalDevice.createCommandPool(poolInfo);
}

void DrawCommand::CreateCommandBuffers(uint32_t commandSize, vk::CommandPool commandPool)
{
    if (m_LogicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("�R�}���h�o�b�t�@���쐬���悤�Ƃ��܂������A�_���f�o�C�X��NULL�ł�");

    if (m_CommandPool == VK_NULL_HANDLE)
        throw std::runtime_error("�R�}���h�o�b�t�@���쐬���悤�Ƃ��܂������A�R�}���h�v�[����NULL�ł�");


    // �R�}���h�o�b�t�@���A���P�[�g(���蓖�Ă�)���߂̏���ݒ肷��
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = commandPool;                  // �R�}���h�o�b�t�@�����蓖�Ă�R�}���h�v�[��
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;   // �R�}���h�o�b�t�@�̎��(PRIMARY: ���ڃL���[�ɑ��M����o�b�t�@)
    cbAllocInfo.commandBufferCount = commandSize;           // ���蓖�Ă�R�}���h�o�b�t�@�̐�

    // �R�}���h�o�b�t�@�����蓖�ĂāA���̃n���h�����o�b�t�@�̔z��Ɋi�[����
    m_CommandBuffers = m_LogicalDevice.allocateCommandBuffers(cbAllocInfo); //�z��ŏ�������肷��



}

vk::SubmitInfo DrawCommand::CreateSubmitInfo(
    std::vector<vk::PipelineStageFlags>* waitStages)
{
    vk::SubmitInfo submitInfo;
    submitInfo.pNext;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffers[m_NextIndex];

    // �摜�̖�������������ꍇ�͓����������s��
    if (m_Swapchain != VK_NULL_HANDLE)
    {
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_SignalSemaphores[m_CurrentIndex];
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_ImageAvailableSemaphores[m_CurrentIndex];
        submitInfo.pWaitDstStageMask = waitStages->data();   
    }

    return submitInfo;
}

uint32_t DrawCommand::AcquireSwapchainNextImage()
{
    if (m_Swapchain == VK_NULL_HANDLE) return 0;

    // �X���b�v�`�F�[�����玟�ɕ`�悷��C���[�W�i�t���[���o�b�t�@�̂悤�Ȃ��́j�̃C���f�b�N�X���擾���܂��B
    uint32_t imageIndex;
    vk::Result result = m_LogicalDevice.acquireNextImageKHR(
        m_Swapchain,                                // �X���b�v�`�F�[��
        std::numeric_limits<uint64_t>::max(),       // �^�C���A�E�g�̐ݒ�(�����ł͖����ҋ@)
        m_ImageAvailableSemaphores[m_CurrentIndex], // �C���[�W���g�p�\�ɂȂ�̂�ʒm����Z�}�t�H
        nullptr,                                    // �t�F���X(�����ł͎g�p���Ȃ��̂�nullptr)
        &imageIndex                                 // �C���[�W�̃C���f�b�N�X���i�[�����
    );

    // �C���[�W�擾�Ɏ��s�����ꍇ�A�G���[���b�Z�[�W�𓊂���
    if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("�X���b�v�`�F�[������C���[�W���擾�ł��܂���ł����I");
    }

    return imageIndex;
}

void DrawCommand::ImageMemoryBarrier(vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
{

    vk::ImageSubresourceRange subresourceRange;
    subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.layerCount = 1;
    subresourceRange.levelCount = 1;
    
    // �C���[�W�o���A�̐ݒ�
    vk::ImageMemoryBarrier imageMemoryBarrier;
    imageMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eNoneKHR;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = m_ImageSet[m_CurrentIndex].color.image;
    imageMemoryBarrier.newLayout = newLayout;
    imageMemoryBarrier.oldLayout = oldLayout;
    imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.subresourceRange = subresourceRange;

    // �p�C�v���C���o���A
    m_CommandBuffers[m_CurrentIndex].pipelineBarrier
    (
        vk::PipelineStageFlagBits::eColorAttachmentOutput, // sourceStage (�����_�����O�̍Ō�)
        vk::PipelineStageFlagBits::eBottomOfPipe,         // destinationStage (���̏���)
        vk::DependencyFlags{},                            // �ˑ��֌W�t���O (���ɂȂ�)
        nullptr,                                          // �������o���A (�Ȃ�)
        nullptr,                                          // �o�b�t�@�o���A (�Ȃ�)
        imageMemoryBarrier                                // �C���[�W�o���A (1��)
    );
}

