#include "DrawCommand.h"



DrawCommand::DrawCommand() : 
    m_LogicalDevice(VK_NULL_HANDLE),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Swapchain(VK_NULL_HANDLE),
    m_ImageSet(nullptr),
    m_CommandBuffers(),
    m_CommandPool(VK_NULL_HANDLE),
    m_RenderFinishedSemaphores(),
    //m_Fences(),
    m_QueueSelector()
{

}

DrawCommand::~DrawCommand()
{
}

void DrawCommand::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
{
    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;
    m_QueueSelector.Initialize(m_PhysicalDevice);

    //�R�}���h�v�[��(�R�}���h��u���̈�)���쐬
    CreateCommandPool();

    //�R�}���h�v�[���ɃR�}���h�o�b�t�@�����蓖��
    CreateCommandBuffers(1, m_CommandPool);

    // �Z�}�t�H�̍쐬
    CreateSemaphore(m_RenderFinishedSemaphores);
    //�t�F���X�̍쐬
    //CreateFence(m_Fences);


}

//void DrawCommand::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, SwapchainRenderer* swapchainRenderer)
//{
//    m_Swapchain = swapchainRenderer->GetSwapchain();
//
//    m_LogicalDevice = logicalDevice;
//    m_PhysicalDevice = physicalDevice;
//    m_ImageSet = swapchainRenderer->GetImageSets();
//    m_QueueSelector.Initialize(m_PhysicalDevice);
//
//    // �g�p����t���[���̖���
//    size_t imageSize = m_ImageSet.size();
//
//    // �t���[���̐������Z�}�t�H�ƃt�F���X���쐬
//    m_RenderFinishedSemaphores.resize(imageSize);
//    m_ImageAvailableSemaphores.resize(imageSize);
//    m_Fences.resize(imageSize);
//    for (size_t i = 0; i < imageSize; i++)
//    {
//        // �Z�}�t�H�̍쐬
//        CreateSemaphore(m_RenderFinishedSemaphores[i]);
//        CreateSemaphore(m_ImageAvailableSemaphores[i]);
//
//        //�t�F���X�̍쐬
//        CreateFence(m_Fences[i]);
//    }
//
//    //�R�}���h�v�[��(�R�}���h��u���̈�)���쐬
//    CreateCommandPool();
//
//    //�R�}���h�v�[���ɃR�}���h�o�b�t�@�����蓖��
//    CreateCommandBuffers(imageSize, m_CommandPool);
//}

void DrawCommand::Destroy()
{
    //�R�}���h�v�[���̉��
    m_LogicalDevice.freeCommandBuffers(
        m_CommandPool,
        1,
        &m_CommandBuffers);

    //�R�}���h�v�[���̔j��
    m_LogicalDevice.destroyCommandPool(m_CommandPool);
}

vk::CommandBuffer DrawCommand::GetBuffer()
{
    return m_CommandBuffers;
}

void DrawCommand::BeginRendering(RenderingImageSet* imageSet, vk::Semaphore imageAvableSemaphore, vk::Rect2D renderArea)
{
    m_ImageSet = imageSet;
    m_ImageAvailableSemaphores = imageAvableSemaphore;

    // �J���[�o�b�t�@�A�^�b�`�����g
    vk::RenderingAttachmentInfo colorAttachment;
    colorAttachment.pNext;
    colorAttachment.imageView = m_ImageSet->color.view;
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.clearValue = vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 1.0f, 1.0f}));
    colorAttachment.resolveImageView = VK_NULL_HANDLE;
    colorAttachment.resolveImageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.resolveMode = vk::ResolveModeFlagBits::eNone;



    // �[�x�o�b�t�@�A�^�b�`�����g�i3D�I�u�W�F�N�g�p�Ɏg�p�j
    vk::RenderingAttachmentInfo depthAttachment;
    depthAttachment.imageView = m_ImageSet->depth.view;
    depthAttachment.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.clearValue = vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0));
    depthAttachment.resolveImageView = VK_NULL_HANDLE;
    depthAttachment.resolveImageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
    depthAttachment.resolveMode = vk::ResolveModeFlagBits::eNone;

    // �[�x�摜�ɃX�e���V�����܂߂�̂ŕK�v�Ȃ�
    //vk::RenderingAttachmentInfo stencilAttachment;

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags;
    beginInfo.pNext;
    beginInfo.pInheritanceInfo; // �Z�J���_���R�}���h�o�b�t�@(�g�p���Ȃ�)


    // �_�C�i�~�b�N�����_�����O�̐ݒ�
    vk::RenderingInfo renderingInfo;
    renderingInfo.pNext;
    renderingInfo.flags;
    renderingInfo.renderArea = renderArea;
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;
    renderingInfo.pDepthAttachment = &depthAttachment;
    renderingInfo.pStencilAttachment = VK_NULL_HANDLE;

    // Dynamic Rendering���J�n
    auto commandBuffer = m_CommandBuffers;
    commandBuffer.begin(beginInfo);
    commandBuffer.beginRendering(renderingInfo);
}

void DrawCommand::EndRendering(vk::Fence fence, vk::ImageLayout newImageLayout)
{
    // �`��R�}���h�̋L�^���I������
    m_CommandBuffers.endRendering();

    // �R�}���h�̏I���O�ɃC���[�W�̏�Ԃ��g�p�ړI�ɍ��킹�ĕύX����
    ImageMemoryBarrier(m_ImageSet->color.buffer, vk::ImageLayout::eUndefined, newImageLayout);

    // �R�}���h�̋L�^�����
    m_CommandBuffers.end();

    // ���M���̍쐬
    std::vector<vk::PipelineStageFlags> waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    vk::SubmitInfo submitInfo = CreateSubmitInfo(&waitStages);

    // �L���[�ɃR�}���h�𑗐M
    vk::Queue queue = m_LogicalDevice.getQueue(m_QueueSelector.GetGraphicIndex(), 0);
    queue.submit(submitInfo, fence);
}

//vk::Semaphore DrawCommand::GetImageAvableSemaphore()
//{
//    return m_ImageAvailableSemaphores;
//}

vk::Semaphore DrawCommand::GetSignalSemaphore()
{
    return m_RenderFinishedSemaphores;
}

//vk::Fence DrawCommand::GetFence()
//{
//    return m_Fences;
//}

//void DrawCommand::WaitFence()
//{
//    std::vector<vk::Fence> usingFences = { m_Fences };
//    m_LogicalDevice.waitForFences(
//        usingFences,							// ���p����t�F���X�B
//        VK_TRUE,								// �t�F���X���S�ăV�O�i����ԂɂȂ�܂ő҂�
//        UINT64_MAX);							// �ő�ҋ@����
//
//    m_LogicalDevice.resetFences(usingFences);	// �t�F���X���V�O�i����Ԃɂ���
//}


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
    vk::CommandBufferAllocateInfo allocateInfo;
    allocateInfo.commandPool = commandPool;                  // �R�}���h�o�b�t�@�����蓖�Ă�R�}���h�v�[��
    allocateInfo.level = vk::CommandBufferLevel::ePrimary;   // �R�}���h�o�b�t�@�̎��(PRIMARY: ���ڃL���[�ɑ��M����o�b�t�@)
    allocateInfo.commandBufferCount = commandSize;           // ���蓖�Ă�R�}���h�o�b�t�@�̐�

    // �R�}���h�o�b�t�@�����蓖�ĂāA���̃n���h�����o�b�t�@�̔z��Ɋi�[����
    m_CommandBuffers = m_LogicalDevice.allocateCommandBuffers(allocateInfo).front(); //�z��ŏ�������肷��



}

vk::SubmitInfo DrawCommand::CreateSubmitInfo(
    std::vector<vk::PipelineStageFlags>* waitStages)
{
    vk::SubmitInfo submitInfo;
    submitInfo.pNext;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffers;

    // �摜�̖�������������ꍇ�͓����������s��
    if (m_Swapchain != VK_NULL_HANDLE)
    {
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_RenderFinishedSemaphores;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_ImageAvailableSemaphores;
        submitInfo.pWaitDstStageMask = waitStages->data();   
    }

    return submitInfo;
}

void DrawCommand::ImageMemoryBarrier(vk::Image& image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
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
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.newLayout = newLayout;
    imageMemoryBarrier.oldLayout = oldLayout;
    imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.subresourceRange = subresourceRange;

    // �p�C�v���C���o���A
    m_CommandBuffers.pipelineBarrier
    (
        vk::PipelineStageFlagBits::eColorAttachmentOutput, // sourceStage (�����_�����O�̍Ō�)
        vk::PipelineStageFlagBits::eBottomOfPipe,         // destinationStage (���̏���)
        vk::DependencyFlags{},                            // �ˑ��֌W�t���O (���ɂȂ�)
        nullptr,                                          // �������o���A (�Ȃ�)
        nullptr,                                          // �o�b�t�@�o���A (�Ȃ�)
        imageMemoryBarrier                                // �C���[�W�o���A (1��)
    );
}

