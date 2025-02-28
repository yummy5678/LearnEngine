#include "VPresentationCommand.h"

VPresentationCommand::VPresentationCommand() : 
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
    m_Swapchain(VK_NULL_HANDLE),
	m_CommandPool(VK_NULL_HANDLE),
	m_CommandBuffers(),
	m_Semaphores(),
    m_SwapChainIndex(0),
    m_QueueFamily()
{
}

VPresentationCommand::~VPresentationCommand()
{
    Cleanup();
}

void VPresentationCommand::Initialize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainKHR swapchain)
{
    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;
    m_Swapchain = swapchain;

    // �X���b�v�`�F�C���̉摜���擾
    std::vector<vk::Image> swapchainImages = logicalDevice.getSwapchainImagesKHR(swapchain);
    // �摜�̖������擾
    size_t imageCount = swapchainImages.size();

    CreateCommandBuffer(imageCount);

    /*m_Semaphores*/


    m_QueueFamily.Initialize(m_PhysicalDevice);
}

void VPresentationCommand::RunningCommand(vk::Semaphore imageAvailableSemaphore)
{
    m_SwapChainIndex = AcquireSwapchainNextImage(imageAvailableSemaphore);

    vk::PresentInfoKHR presentInfo;

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_Swapchain;
    presentInfo.pImageIndices = &m_SwapChainIndex;

    // �g�p����L���[(�O���t�B�b�N�L���[��v���[���g�L���[�Ȃ�)�̃C���f�b�N�X���擾
    auto graphicsQueue = m_LogicalDevice.getQueue(m_QueueFamily.GetGraphicIndex(), 0);

    if (graphicsQueue.presentKHR(presentInfo) != vk::Result::eSuccess)
    {
        throw std::runtime_error("�X���b�v�`�F�[���̉摜�̕\���Ɏ��s���܂����I");
    }
}

void VPresentationCommand::Cleanup()
{
}

void VPresentationCommand::CreateCommandPool()
{
    if (m_LogicalDevice == VK_NULL_HANDLE || m_PhysicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("�f�o�C�X�����쐬�ł��B�R�}���h�v�[���̍쐬�Ɏ��s���܂����I");
        return;
    };

    if (m_CommandPool != VK_NULL_HANDLE)
    {
        throw std::runtime_error("���ɃR�}���h�v�[���͍쐬����Ă��܂��I");
        return;
    }
    QueueFamilySelector queueFamily;
    queueFamily.Initialize(m_PhysicalDevice);

    // �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.pNext = nullptr;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = queueFamily.GetGraphicIndex();


    // �O���t�B�b�N�X�L���[�t�@�~���[�p�̃R�}���h�v�[�����쐬����
    m_CommandPool = m_LogicalDevice.createCommandPool(poolInfo);
}

void VPresentationCommand::CreateCommandBuffer(size_t commandSize)
{
    CreateCommandPool();

    // �R�}���h�o�b�t�@���A���P�[�g(���蓖�Ă�)���߂̏���ݒ肷��
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = m_CommandPool;                  // �R�}���h�o�b�t�@�����蓖�Ă�R�}���h�v�[��
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;   // �R�}���h�o�b�t�@�̎��(PRIMARY: ���ڃL���[�ɑ��M����o�b�t�@)
    cbAllocInfo.commandBufferCount = commandSize;           // ���蓖�Ă�R�}���h�o�b�t�@�̐�

    // �R�}���h�o�b�t�@�����蓖�ĂāA���̃n���h�����o�b�t�@�̔z��Ɋi�[����
    m_CommandBuffers = m_LogicalDevice.allocateCommandBuffers(cbAllocInfo); //�z��ŏ�������肷��

}

const uint32_t VPresentationCommand::AcquireSwapchainNextImage(vk::Semaphore imageAvailableSemaphore)
{
    // �X���b�v�`�F�[�����玟�ɕ`�悷��C���[�W�i�t���[���o�b�t�@�̂悤�Ȃ��́j�̃C���f�b�N�X���擾���܂��B
    uint32_t imageIndex;
    vk::Result result = m_LogicalDevice.acquireNextImageKHR(
        m_Swapchain,                            // �X���b�v�`�F�[��
        std::numeric_limits<uint64_t>::max(),   // �^�C���A�E�g�̐ݒ�(�����ł͖����ҋ@)
        imageAvailableSemaphore,                // �C���[�W���g�p�\�ɂȂ�̂�ʒm����Z�}�t�H
        nullptr,                                // �t�F���X(�����ł͎g�p���Ȃ��̂�nullptr)
        &imageIndex                             // �C���[�W�̃C���f�b�N�X���i�[�����
    );

    // �C���[�W�擾�Ɏ��s�����ꍇ�A�G���[���b�Z�[�W�𓊂���
    if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("�X���b�v�`�F�[������C���[�W���擾�ł��܂���ł����I");
    }

    return imageIndex;
}
