#include "CImagesGenerator.h"

CImagesGenerator::CImagesGenerator()
{
	m_ClassName = "ImagesGenerator";
}

CImagesGenerator::~CImagesGenerator()
{
}


void CImagesGenerator::Create(uint32_t ImageNum, vk::Extent3D extent, vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
{
    m_bCreated = true;

    m_LogicalDevice = logicalDevice;
    m_Images.resize(ImageNum);
    auto imageInfo = CreateImageInfo(extent);
    for (auto image : m_Images)
    {
        image = m_LogicalDevice.createImage(imageInfo);
    }

    m_ImageViews.resize(ImageNum);
    std::vector<vk::ImageViewCreateInfo> viewInfo;
    viewInfo.resize(ImageNum);

    for (int i = 0;i < ImageNum;i++)
    {
        auto viewInfo = CreateImageViewInfo(m_Images[i]);
        m_ImageViews[i] = m_LogicalDevice.createImageView(viewInfo);
    }


}

void CImagesGenerator::Destroy()
{
	//���g���쐬����Ă��Ȃ��Ȃ����������s��Ȃ�
	if (m_bCreated == false) return;
	m_bCreated = false;

	//�C���[�W�̉��
	for (auto imageView : m_ImageViews)
	{
        m_LogicalDevice.destroyImageView(imageView);
	}


    for (auto image : m_Images)
    {
        m_LogicalDevice.destroyImage(image);
    }

    m_LogicalDevice.freeMemory(m_ImageMemory);
}

std::vector<vk::Image> CImagesGenerator::GetSwapChainImages()
{
	CheckCreated();
	return m_Images;
}

std::vector<vk::ImageView> CImagesGenerator::GetSwapChainImageViews()
{
    CheckCreated();
    return m_ImageViews;
}


vk::ImageCreateInfo CImagesGenerator::CreateImageInfo(vk::Extent3D extent)
{
    vk::ImageCreateInfo imageCreateInfo;
    imageCreateInfo.pNext;
    imageCreateInfo.flags;
    imageCreateInfo.imageType = vk::ImageType::e2D;
    imageCreateInfo.format = vk::Format::eR8G8B8A8Unorm;    // �C���[�W�̃t�H�[�}�b�g
    imageCreateInfo.extent.width = extent.width;            // �C���[�W�̕�
    imageCreateInfo.extent.height = extent.height;          // �C���[�W�̍���
    imageCreateInfo.extent.depth = extent.depth;            // �C���[�W�̐[��
    imageCreateInfo.mipLevels = 1;                          // �~�b�v�}�b�v���x��
    imageCreateInfo.arrayLayers = 1;                        // ���C���[��
    imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // �T���v����
    imageCreateInfo.tiling = vk::ImageTiling::eOptimal;     // �^�C�����O����
    imageCreateInfo.usage = vk::ImageUsageFlagBits::eColorAttachment;   // �g�p���@
    imageCreateInfo.sharingMode = vk::SharingMode::eExclusive;          // ���L���[�h
    imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;        // �������C�A�E�g

    return imageCreateInfo;
}

vk::ImageViewCreateInfo CImagesGenerator::CreateImageViewInfo(vk::Image image)
{
    // �摜�r���[�쐬���̏�����
    vk::ImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.image = image;                                            // View ���쐬���邽�߂� Image
    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;                         // Image �̎�� (1D, 2D, 3D, Cube �Ȃ�)
    imageViewCreateInfo.format = vk::Format::eB8G8R8A8Unorm;                                          // Image �f�[�^�̃t�H�[�}�b�g
    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;             // RGBA �R���|�[�l���g�𑼂� RGBA �l�Ƀ��}�b�v���邱�Ƃ��ł��܂�
    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;

    // Subresource �� Image �̈ꕔ������\�����邽�߂̐ݒ�ł�
    imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;  // Image �̂ǂ̖ʂ�\�����邩 (��: COLOR_BIT �͐F��\�����邽��)
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;          // �\�����J�n���� Mipmap ���x��
    imageViewCreateInfo.subresourceRange.levelCount = 1;            // �\������ Mipmap ���x���̐�
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;        // �\�����J�n����z�񃌃x��
    imageViewCreateInfo.subresourceRange.layerCount = 1;            // �\������z�񃌃x���̐�

}

vk::DeviceMemory CImagesGenerator::AllocateImageMemory(vk::Device device, vk::PhysicalDevice physicalDevice, vk::Image image)
{
    // �C���[�W�̃������v�����擾
    vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(image);

    // �������̊��蓖�ď���ݒ�
    vk::MemoryAllocateInfo allocInfo;
    // �z�X�g���̃������ɒu����ACPU ����ǂݏ����ł��郁������T��
    vk::MemoryPropertyFlags properties = vk::MemoryPropertyFlagBits::eDeviceLocal;
    allocInfo.allocationSize = memoryRequirements.size;  // �������T�C�Y
    allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, properties);

    // ���������m��
    vk::DeviceMemory imageMemory;
    try 
    {
        imageMemory = device.allocateMemory(allocInfo);
    }
    catch (vk::SystemError& err) {
        throw std::runtime_error("�C���[�W�������̊m�ۂɎ��s���܂���!");
    }

    // �C���[�W�Ƀ��������o�C���h
    device.bindImageMemory(image, imageMemory, 0);

    return imageMemory;
}

uint32_t CImagesGenerator::FindMemoryType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
        {
            return i;
        }
    }

    throw std::runtime_error("�K�؂ȃ������^�C�v���������܂���ł���!");
}
