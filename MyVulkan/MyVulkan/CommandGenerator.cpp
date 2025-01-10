#include "CommandGenerator.h"


SwapChainCommandGenerator::SwapChainCommandGenerator():
    m_LogicalDevice(nullptr),
    m_PhysicalDevice(nullptr),
    m_CommandPool(nullptr),
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
    m_SignalSemaphores  = m_SemaphoreGenerator.GetSignalSemaphores();
    m_WaitSemaphores    = m_SemaphoreGenerator.GetWaitSemaphores();

    //�t�F���X�̍쐬
    m_FenceGenerator.Create(logicalDevice, commandSize);
    m_Fences            = m_FenceGenerator.GetFence();

    //�R�}���h�v�[��(�R�}���h��u���̈�)���쐬
    m_CommandPool       = CreateCommandPool(logicalDevice, physicalDevice);

    //�R�}���h�v�[���ɃR�}���h�o�b�t�@�����蓖��
    m_CommandBuffers    = CreateCommandBuffers(logicalDevice, commandSize, m_CommandPool);




}

void SwapChainCommandGenerator::Destroy()
{
    //�R�}���h�v�[���̉��
    m_LogicalDevice.freeCommandBuffers(
        m_CommandPool,
        (uint32_t)m_CommandBuffers.size(),
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


void SwapChainCommandGenerator::PresentFrame(vk::SwapchainKHR swapchain, uint32_t commandIndex)
{
    vk::PresentInfoKHR presentInfo;

    presentInfo.swapchainCount =    1;
    presentInfo.pSwapchains =       &swapchain;
    presentInfo.pImageIndices =     &commandIndex;

    // �g�p����L���[(�O���t�B�b�N�L���[��v���[���g�L���[�Ȃ�)�̃C���f�b�N�X���擾
    auto queueFamily = QueueFamilySelector(m_PhysicalDevice);
    auto graphicsQueue = m_LogicalDevice.getQueue(queueFamily.GetGraphicIndex(), 0);

    if (graphicsQueue.presentKHR(presentInfo) != vk::Result::eSuccess)
    {
        throw std::runtime_error("�X���b�v�`�F�[���̉摜�̕\���Ɏ��s���܂����I");
    }
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
    submitInfo.signalSemaphoreCount = (uint32_t)m_SignalSemaphores.size();
    submitInfo.pSignalSemaphores = m_SignalSemaphores.data();
    submitInfo.allowDuplicate;
    submitInfo.pWaitDstStageMask;
    submitInfo.waitSemaphoreCount = (uint32_t)m_WaitSemaphores.size();
    submitInfo.pWaitSemaphores = m_WaitSemaphores.data();
    submitInfo.commandBufferCount = (uint32_t)commandBuffers.size();
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



