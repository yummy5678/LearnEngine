#include "ImagesGenerator.h"

CImagesGenerator::CImagesGenerator()
{
	m_ClassName = "ImagesGenerator";
}

CImagesGenerator::~CImagesGenerator()
{
}


void CImagesGenerator::Create(uint32_t ImageNum, vk::Extent2D extent, vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
{
    m_bCreated = true;

    m_LogicalDevice = logicalDevice;
    m_Size = ImageNum;

    m_Images.resize(ImageNum);
    m_ImageMemory.resize(ImageNum);
    m_ImageViews.resize(ImageNum);

    std::vector<vk::ImageViewCreateInfo> viewInfo;
    viewInfo.resize(ImageNum);


    m_ImageInfo = CreateImageInfo(extent,m_Fomat);
    for (uint32_t i = 0; i < ImageNum; i++)
    {
        //�摜���쐬
        m_Images[i] = m_LogicalDevice.createImage(m_ImageInfo);

        //�摜�̃��������m��
        auto allocInfo = AllocateImageMemory(m_Images[i], logicalDevice, physicalDevice);
        m_ImageMemory[i] = m_LogicalDevice.allocateMemory(allocInfo);
        m_LogicalDevice.bindImageMemory(m_Images[i], m_ImageMemory[i], 0);  // �o�C���h

        //�摜�������ۂ̏���ݒ�
        auto viewInfo = CreateImageViewInfo(m_Images[i], m_Fomat);
        m_ImageViews[i] = m_LogicalDevice.createImageView(viewInfo);
    }
}

void CImagesGenerator::CreateForSurface(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    m_bCreated = true;
    m_LogicalDevice = logicalDevice;

    // �T�[�t�F�X�̋@�\���擾
    auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
    std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface);

    int imageCount = surfaceCapabilities.minImageCount + 1;
    vk::Extent2D extent = surfaceCapabilities.currentExtent;
    m_Fomat = surfaceFormats[0].format;

    m_Images.resize(imageCount);
    m_ImageMemory.resize(imageCount);
    m_ImageViews.resize(imageCount);

    std::vector<vk::ImageViewCreateInfo> viewInfo;
    viewInfo.resize(imageCount);


    m_ImageInfo = CreateImageInfo(extent, m_Fomat);
    for (uint32_t i = 0; i < imageCount; i++)
    {
        //�摜���쐬
        m_Images[i] = m_LogicalDevice.createImage(m_ImageInfo);

        //�摜�̃��������m��
        auto allocInfo = AllocateImageMemory(m_Images[i], logicalDevice, physicalDevice);
        m_ImageMemory[i] = m_LogicalDevice.allocateMemory(allocInfo);
        m_LogicalDevice.bindImageMemory(m_Images[i], m_ImageMemory[i], 0);  // �o�C���h

        //�摜�������ۂ̏���ݒ�
        auto viewInfo = CreateImageViewInfo(m_Images[i], m_Fomat);
        m_ImageViews[i] = m_LogicalDevice.createImageView(viewInfo);
    }
}

void CImagesGenerator::Destroy()
{
	//���g���쐬����Ă��Ȃ��Ȃ����������s��Ȃ�
	if (m_bCreated == false) return;
	m_bCreated = false;

	// �C���[�W�r���[�̉��
	for (auto& imageView : m_ImageViews)
	{
        m_LogicalDevice.destroyImageView(imageView);
	}

    // �C���[�W�̉��
    for (auto& image : m_Images)
    {
        m_LogicalDevice.destroyImage(image);
    }

    // �������[�̉��
    for (auto& memory : m_ImageMemory)
    {
        m_LogicalDevice.freeMemory(memory);
    }
}

std::vector<vk::Image> CImagesGenerator::GetImages()
{
	CheckCreated();
	return m_Images;
}

std::vector<vk::ImageView> CImagesGenerator::GetImageViews()
{
    CheckCreated();
    return m_ImageViews;
}

vk::ImageCreateInfo CImagesGenerator::GetImageInfo()
{
    CheckCreated();
    return m_ImageInfo;
}

vk::Format CImagesGenerator::GetFomat()
{
    CheckCreated();
    return m_Fomat;
}

uint32_t CImagesGenerator::GetSize()
{
    return m_Size;
}


vk::ImageCreateInfo CImagesGenerator::CreateImageInfo(vk::Extent2D extent, vk::Format fomat)
{
    vk::ImageCreateInfo imageCreateInfo;
    imageCreateInfo.pNext;
    imageCreateInfo.flags;
    imageCreateInfo.imageType = vk::ImageType::e2D;
    imageCreateInfo.format = fomat;                         // �C���[�W�̃t�H�[�}�b�g
    imageCreateInfo.extent.width = extent.width;            // �C���[�W�̕�
    imageCreateInfo.extent.height = extent.height;          // �C���[�W�̍���
    imageCreateInfo.extent.depth = 1;                       // �C���[�W�̉��s��
    imageCreateInfo.mipLevels = 1;                          // �~�b�v�}�b�v���x��
    imageCreateInfo.arrayLayers = 1;                        // ���C���[��
    imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // �T���v����
    imageCreateInfo.tiling = vk::ImageTiling::eOptimal;     // �^�C�����O����
    imageCreateInfo.usage = vk::ImageUsageFlagBits::eColorAttachment;   // �g�p���@
    imageCreateInfo.sharingMode = vk::SharingMode::eExclusive;          // ���L���[�h
    imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;        // �������C�A�E�g

    return imageCreateInfo;
}

vk::ImageViewCreateInfo CImagesGenerator::CreateImageViewInfo(vk::Image image, vk::Format fomat)
{
    // �摜�r���[�쐬���̏�����
    vk::ImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.pNext;
    imageViewCreateInfo.flags;
    imageViewCreateInfo.image = image;                                  // View ���쐬���邽�߂� Image
    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;              // Image �̎�� (1D, 2D, 3D, Cube �Ȃ�)
    imageViewCreateInfo.format = fomat;                                 // Image �f�[�^�̃t�H�[�}�b�g(image�Ɠ������̂łȂ���΂Ȃ�Ȃ�)
    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity; // RGBA �R���|�[�l���g�𑼂� RGBA �l�Ƀ��}�b�v���邱�Ƃ��ł��܂�
    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;

    // Subresource �� Image �̈ꕔ������\�����邽�߂̐ݒ�ł�
    imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;  // Image �̂ǂ̖ʂ�\�����邩 (��: COLOR_BIT �͐F��\�����邽��)
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;          // �\�����J�n���� Mipmap ���x��
    imageViewCreateInfo.subresourceRange.levelCount = 1;            // �\������ Mipmap ���x���̐�
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;        // �\�����J�n����z�񃌃x��
    imageViewCreateInfo.subresourceRange.layerCount = 1;            // �\������z�񃌃x���̐�

    return imageViewCreateInfo;
}

vk::MemoryAllocateInfo CImagesGenerator::AllocateImageMemory(vk::Image image, vk::Device device, vk::PhysicalDevice physicalDevice)
{
    // �C���[�W�̃������v�����擾
    vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(image);

    // �������̊��蓖�ď���ݒ�
    vk::MemoryAllocateInfo allocateInfo;
    allocateInfo.pNext;
    allocateInfo.allocationSize = memoryRequirements.size;  // �摜�̃������T�C�Y
    allocateInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memoryRequirements.memoryTypeBits);   // �������^�C�v

    return allocateInfo;
}

uint32_t CImagesGenerator::FindMemoryType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter)
{
    // �����f�o�C�X���烁�����v���p�e�B���擾�B
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

    // �T���Ă��郁�����v���p�e�B�̃t���O
    vk::MemoryPropertyFlags properties = vk::MemoryPropertyFlagBits::eDeviceLocal;

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        // typeFilter�́A�r�b�g�}�X�N�Ń������^�C�v���t�B���^�����O���邽�߂̂��́B
        // �����ł́AtypeFilter�̃r�b�g��i�Ԗڂ̃������^�C�v�ɑΉ����邩�ǂ������m�F���Ă��܂��B
        // (1 << i)�́Ai�r�b�g�ڂ̃r�b�g��1�ɂ����r�b�g�}�X�N�ł��B
        // typeFilter & (1 << i) �́Ai�r�b�g�ڂ̃r�b�g�������Ă��邩���m�F���܂��B
        if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    // �G���[���b�Z�[�W
    throw std::runtime_error("�K�؂ȃ������^�C�v���������܂���ł���!");
}
