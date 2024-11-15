#include "SwapChainRenderer.h"




SwapchainRenderer::SwapchainRenderer(VulkanInitializer* initializer)
{
    m_ClassName = "SwapchainGenerator";


    initializer->GetPDeviceExtension()->UseSwapchain();
}

SwapchainRenderer::~SwapchainRenderer()
{

}

void SwapchainRenderer::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    std::cout  << m_ClassName << "���쐬" << std::endl;
    m_bCreated = true;
    m_LogicalDevice = logicalDevice;
    m_SwapchainInfo = CreateSwapchainInfo(physicalDevice, surface);

    m_Swapchain = logicalDevice.createSwapchainKHR(m_SwapchainInfo);

    m_SwapChainImages.Create(logicalDevice, physicalDevice, m_Swapchain, m_SwapchainInfo);

    //�R�}���h�o�b�t�@�̍쐬
    m_CommandGenerator.Create(logicalDevice, physicalDevice, m_SwapchainInfo.minImageCount);
}

void SwapchainRenderer::Destroy(vk::Device logicalDevice)
{
    //���g���쐬����Ă��Ȃ��Ȃ����������s��Ȃ�
    if (m_bCreated == false) return;
    m_bCreated = false;
    
    //�X���b�v�`�F�[���̉��
    m_LogicalDevice.destroySwapchainKHR(m_Swapchain);
}

vk::SwapchainKHR SwapchainRenderer::GetSwapchain()
{
    CheckCreated();
    return m_Swapchain;
}

vk::SwapchainCreateInfoKHR SwapchainRenderer::GetSwapchainInfo()
{
    CheckCreated();
    return m_SwapchainInfo;
}

SwapChainImage SwapchainRenderer::GetImages()
{
    CheckCreated();
    return m_SwapChainImages;
}

vk::Extent2D SwapchainRenderer::GetFrameExtent()
{
    return m_SwapchainInfo.imageExtent;
}

void SwapchainRenderer::UpdateFrame(std::vector<RenderingUnit> renderingUnits)
{
    vk::ResultValue acquire = m_LogicalDevice.acquireNextImageKHR(m_Swapchain, std::numeric_limits<uint64_t>::max(), {}, nullptr);
    if (acquire.result != vk::Result::eSuccess) std::cerr << "���t���[���̎擾�Ɏ��s���܂����B" << std::endl;

    m_CommandGenerator.DrawFrame(
        acquire.value,
        renderingUnits,
        m_SwapChainImages.GetColorImageViews()[acquire.value],
        m_SwapChainImages.GetColorImageViews()[acquire.value]);

    m_CommandGenerator.PresentFrame(m_Swapchain, acquire.value);
}

/// <summary>
/// �X���b�v�`�F�[���C���t�H�̍쐬
/// </summary>
vk::SwapchainCreateInfoKHR SwapchainRenderer::CreateSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    // �X���b�v�`�F�[���̐ݒ���s��
    // �T�[�t�F�X�̋@�\���擾
    auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
    std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface);
    std::vector<vk::PresentModeKHR> surfacePresentModes = physicalDevice.getSurfacePresentModesKHR(surface);

    // �T�[�t�F�X����擾�������̒�����œK�Ȃ��̂�I��
    vk::SurfaceFormatKHR format = SelectSurfaceFormat(surfaceFormats);
    vk::PresentModeKHR presentMode = SelectPresentMode(surfacePresentModes);

    // �X���b�v�`�F�[���ɕK�v�ȃC���[�W�̐�������
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (imageCount > surfaceCapabilities.maxImageCount)
    {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    // �X���b�v�`�F�[���쐬���̐ݒ�
    vk::SwapchainCreateInfoKHR swapchainInfo;
    swapchainInfo.pNext;                       // �g���`�F�[���ւ̃|�C���^
    swapchainInfo.flags;                       // �쐬�t���O
    swapchainInfo.surface = surface;           // �X���b�v�`�F�[�����^�[�Q�b�g�Ƃ���T�[�t�F�X
    swapchainInfo.minImageCount = imageCount;  // �X���b�v�`�F�[���Ɋ܂܂��ŏ��摜��
    swapchainInfo.imageFormat = format.format;          // �摜�t�H�[�}�b�g
    swapchainInfo.imageColorSpace = format.colorSpace;  // �J���[�X�y�[�X
    swapchainInfo.imageExtent = surfaceCapabilities.currentExtent;         // �摜�̕��ƍ���
    swapchainInfo.imageArrayLayers = 1;                                    // �摜�̔z��w��(���ʉ摜�Ȃ�"1"�ɂ���)
    swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;   // �摜�̎g�p�p�r
    swapchainInfo.imageSharingMode = vk::SharingMode::eExclusive;          // �摜�̋��L���[�h�i�����ݒ�͔r���I���[�h�j
    swapchainInfo.queueFamilyIndexCount;                                   // �L���[�t�@�~���[�C���f�b�N�X�̐�
    swapchainInfo.pQueueFamilyIndices;                                     // �L���[�t�@�~���[�C���f�b�N�X�̔z��ւ̃|�C���^
    swapchainInfo.preTransform = surfaceCapabilities.currentTransform;     // �T�[�t�F�X�̃g�����X�t�H�[��(�ό`���)
    swapchainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque; // �A���t�@�������[�h
    swapchainInfo.presentMode = presentMode;        // �v���[���e�[�V�������[�h
    swapchainInfo.clipped = VK_TRUE;                // �N���b�v����邩�ǂ���
    swapchainInfo.oldSwapchain = VK_NULL_HANDLE;    // �Â��X���b�v�`�F�[���ւ̃|�C���^

    //////////////////////////////
    // �L���[�̋��L�ݒ�
    //////////////////////////////
    // �L���[�t�@�~�����قȂ�ꍇ�͋��L���[�h��ݒ�
    QueueFamilySelector queueFamilySelector(physicalDevice);
    if (queueFamilySelector.GetGraphicIndex() != queueFamilySelector.GetPresentationIndex(surface))
    {
        std::vector<uint32_t> queueFamilyIndices = { queueFamilySelector.GetGraphicIndex(), queueFamilySelector.GetPresentationIndex(surface) };
        swapchainInfo.imageSharingMode = vk::SharingMode::eConcurrent;      // ���s���[�h�ɐݒ�
        swapchainInfo.queueFamilyIndexCount = queueFamilyIndices.size();    // �L���[�t�@�~���[�C���f�b�N�X�̐���ݒ�
        swapchainInfo.pQueueFamilyIndices = queueFamilyIndices.data();      // �L���[�t�@�~���[�C���f�b�N�X�̔z���ݒ�
    }

    return swapchainInfo;
}

vk::SurfaceFormatKHR SwapchainRenderer::SelectSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&             
            availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)    
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

vk::PresentModeKHR SwapchainRenderer::SelectPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        // �T�[�t�F�X�̎g�p�\�ȃ��[�h�̒���
        // ���[���{�b�N�X���[�h������΂���ɂ���
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {

            return availablePresentMode;
        }
    }
    // �������FIFO�L���[���g�p����(�K���T�|�[�g����Ă���)
    return vk::PresentModeKHR::eFifo; 
}
