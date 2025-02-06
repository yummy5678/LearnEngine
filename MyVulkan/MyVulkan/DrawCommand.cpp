#include "DrawCommand.h"



DrawCommand::DrawCommand() : 
    m_pLogicalDevice(nullptr),
    m_pPhysicalDevice(VK_NULL_HANDLE),
    m_ImageDrawIndex(0),
    m_ImageSet(),
    m_CommandBuffers(),
    m_CommandPool(VK_NULL_HANDLE),
    m_QueueSelector(m_pPhysicalDevice)
{

}

DrawCommand::~DrawCommand()
{
}

void DrawCommand::Create(vk::Device* pLogicalDevice, vk::PhysicalDevice* pPhysicalDevice, std::vector<ImageViewSet> imageSet)
{
    m_pLogicalDevice = pLogicalDevice;
    m_pPhysicalDevice = pPhysicalDevice;
    m_ImageSet = imageSet;

    //// �Z�}�t�H�̍쐬
    //m_SemaphoreGenerator.Create(pLogicalDevice, commandSize);
    //m_SignalSemaphores = m_SemaphoreGenerator.GetSignalSemaphores();
    //m_WaitSemaphores = m_SemaphoreGenerator.GetWaitSemaphores();

    ////�t�F���X�̍쐬
    //m_FenceGenerator.Create(pLogicalDevice, commandSize);
    //m_Fences = m_FenceGenerator.GetFence();

    //�R�}���h�v�[��(�R�}���h��u���̈�)���쐬
    CreateCommandPool(pLogicalDevice);

    //�R�}���h�v�[���ɃR�}���h�o�b�t�@�����蓖��
    CreateCommandBuffers(pLogicalDevice, imageSet.size(), m_CommandPool);

}

void DrawCommand::Destroy()
{
    //�R�}���h�v�[���̉��
    m_pLogicalDevice->freeCommandBuffers(
        m_CommandPool,
        (uint32_t)m_CommandBuffers.size(),
        m_CommandBuffers.data());

    //�R�}���h�v�[���̔j��
    m_pLogicalDevice->destroyCommandPool(m_CommandPool);
}

vk::CommandBuffer DrawCommand::GetBuffer()
{
    if (m_CommandBuffers.empty() == true) 
        throw std::runtime_error("�R�}���h�����o�����Ƃ��܂������A����͂܂����s����Ă��܂���");

    return m_CommandBuffers[m_ImageDrawIndex];
}

void DrawCommand::BeginRendering(uint32_t index, vk::Rect2D renderArea)
{
    if (m_ImageSet.empty() == true) return; // �`�悷��ׂ̃C���[�W���Z�b�g����Ă��Ȃ���Ή������Ȃ�

    // �C���f�b�N�X�̃J�E���g��i�߂�
    m_ImageDrawIndex = index % m_ImageSet.size();   //  �Y�������R�}���h���͈͓̔��Ɏ��܂�悤����

    // �J���[�o�b�t�@�A�^�b�`�����g
    vk::RenderingAttachmentInfo colorAttachment;
    colorAttachment.imageView = m_ImageSet[m_ImageDrawIndex].color;
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.clearValue = vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}));

    // Depth�o�b�t�@�A�^�b�`�����g�i3D�I�u�W�F�N�g�p�Ɏg�p�j
    vk::RenderingAttachmentInfo depthAttachment;
    depthAttachment.imageView = m_ImageSet[m_ImageDrawIndex].depth;
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
    auto commandBuffer = m_CommandBuffers[m_ImageDrawIndex];
    commandBuffer.begin(beginInfo);
    commandBuffer.beginRendering(renderingInfo);



}

void DrawCommand::EndRendering()
{
    m_CommandBuffers[m_ImageDrawIndex].endRendering();
    m_CommandBuffers[m_ImageDrawIndex].end();

    // �L���[�ɃR�}���h�𑗐M
    vk::Queue queue = m_pLogicalDevice->getQueue(m_QueueSelector.GetGraphicIndex(), 0);
    queue.submit(CreateSubmitInfo(m_CommandBuffers[m_ImageDrawIndex]), m_Fences[m_ImageDrawIndex]);
}


void DrawCommand::CreateCommandPool(vk::Device* pLogicalDevice)
{
    if(m_pLogicalDevice == nullptr || *m_pLogicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("�R�}���h�v�[�����쐬���悤�Ƃ��܂������A�_���f�o�C�X��NULL�ł�");

    if (m_pPhysicalDevice == nullptr || *m_pPhysicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("�R�}���h�v�[�����쐬���悤�Ƃ��܂������A�����f�o�C�X��NULL�ł�");

    // �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
    VkCommandPoolCreateInfo poolInfo;
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;	// �R�}���h�o�b�t�@�̃��Z�b�g��������ꍇ�̓t���O��ǉ�����
    poolInfo.queueFamilyIndex = m_QueueSelector.GetGraphicIndex();	            // ���̃R�}���h�v�[�����g�p����L���[�t�@�~���[

   m_CommandPool = pLogicalDevice->createCommandPool(poolInfo);
}

void DrawCommand::CreateCommandBuffers(vk::Device* pLogicalDevice, uint32_t commandSize, vk::CommandPool commandPool)
{
    if (m_pLogicalDevice == nullptr || *m_pLogicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("�R�}���h�o�b�t�@���쐬���悤�Ƃ��܂������A�_���f�o�C�X��NULL�ł�");

    if (m_CommandPool == VK_NULL_HANDLE)
        throw std::runtime_error("�R�}���h�o�b�t�@���쐬���悤�Ƃ��܂������A�R�}���h�v�[����NULL�ł�");


    // �R�}���h�o�b�t�@���A���P�[�g(���蓖�Ă�)���߂̏���ݒ肷��
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = commandPool;                  // �R�}���h�o�b�t�@�����蓖�Ă�R�}���h�v�[��
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;   // �R�}���h�o�b�t�@�̎��(PRIMARY: ���ڃL���[�ɑ��M����o�b�t�@)
    cbAllocInfo.commandBufferCount = commandSize;           // ���蓖�Ă�R�}���h�o�b�t�@�̐�

    // �R�}���h�o�b�t�@�����蓖�ĂāA���̃n���h�����o�b�t�@�̔z��Ɋi�[����
    m_CommandBuffers = pLogicalDevice->allocateCommandBuffers(cbAllocInfo); //�z��ŏ�������肷��

}

vk::SubmitInfo DrawCommand::CreateSubmitInfo(std::vector<vk::CommandBuffer>& commandBuffers, std::vector<vk::Semaphore>& signalSemaphores, std::vector<vk::Semaphore>& waitSemaphores)
{
    vk::SubmitInfo submitInfo;
    submitInfo.pNext;
    submitInfo.signalSemaphoreCount = (uint32_t)m_SignalSemaphores.size();
    submitInfo.pSignalSemaphores = m_SignalSemaphores.data();
    submitInfo.pWaitDstStageMask;
    submitInfo.waitSemaphoreCount = (uint32_t)m_WaitSemaphores.size();
    submitInfo.pWaitSemaphores = m_WaitSemaphores.data();
    submitInfo.commandBufferCount = (uint32_t)commandBuffers.size();
    submitInfo.pCommandBuffers = commandBuffers.data();

    return submitInfo;
}

vk::SubmitInfo DrawCommand::CreateSubmitInfo(vk::CommandBuffer& commandBuffer)
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

//void DrawCommand::RenderMesh(
//    vk::Pipeline pipeline,
//    vk::PipelineLayout pipelineLayout,
//    std::vector<vk::DescriptorSet>* descriptorSets,
//    VMeshObject* drawMesh,
//    Transform* ObjectTransform)
//{
//
//    // �g�p����p�C�v���C�����o�C���h���܂��B
//    m_CommandBuffers[m_ImageDrawIndex].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
//
//    // �v�b�V���萔���V�F�[�_�[�ɓn���܂��B
//    m_CommandBuffers[m_ImageDrawIndex].pushConstants(
//        pipelineLayout,
//        vk::ShaderStageFlagBits::eVertex,   // �v�b�V���萔���X�V����V�F�[�_�[�X�e�[�W
//        0,                                  // �I�t�Z�b�g
//        sizeof(Transform),                  // �v�b�V������f�[�^�̃T�C�Y
//        ObjectTransform                     // ���ۂ̃f�[�^
//    );
//
//
//    // ���_�o�b�t�@���o�C���h
//    m_CommandBuffers[m_ImageDrawIndex].bindVertexBuffers(0, drawMesh->GetSPMesh()->GetVertex().GetBuffer(), {0});
//
//    m_CommandBuffers[m_ImageDrawIndex].bindDescriptorSets(
//        vk::PipelineBindPoint::eGraphics,
//        pipelineLayout,
//        0,
//        *descriptorSets,
//        nullptr);
//
//        // �C���f�b�N�X�o�b�t�@(���_�����ԏ��Ԃ̒l)�����ѕt���܂��B
//    m_CommandBuffers[m_ImageDrawIndex].bindIndexBuffer(drawMesh->GetSPMesh()->GetIndex().GetBuffer(), 0, vk::IndexType::eUint32);
//    m_CommandBuffers[m_ImageDrawIndex].drawIndexed(drawMesh->GetSPMesh()->GetIndex().GetSize(), 1, 0, 0, 0);   // �C���f�b�N�X�ɏ]���ĕ`��
//
//}
