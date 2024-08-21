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
        //画像を作成
        m_Images[i] = m_LogicalDevice.createImage(m_ImageInfo);

        //画像のメモリを確保
        auto allocInfo = AllocateImageMemory(m_Images[i], logicalDevice, physicalDevice);
        m_ImageMemory[i] = m_LogicalDevice.allocateMemory(allocInfo);
        m_LogicalDevice.bindImageMemory(m_Images[i], m_ImageMemory[i], 0);  // バインド

        //画像を扱う際の情報を設定
        auto viewInfo = CreateImageViewInfo(m_Images[i], m_Fomat);
        m_ImageViews[i] = m_LogicalDevice.createImageView(viewInfo);
    }
}

void CImagesGenerator::CreateForSurface(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    m_bCreated = true;
    m_LogicalDevice = logicalDevice;

    // サーフェスの機能を取得
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
        //画像を作成
        m_Images[i] = m_LogicalDevice.createImage(m_ImageInfo);

        //画像のメモリを確保
        auto allocInfo = AllocateImageMemory(m_Images[i], logicalDevice, physicalDevice);
        m_ImageMemory[i] = m_LogicalDevice.allocateMemory(allocInfo);
        m_LogicalDevice.bindImageMemory(m_Images[i], m_ImageMemory[i], 0);  // バインド

        //画像を扱う際の情報を設定
        auto viewInfo = CreateImageViewInfo(m_Images[i], m_Fomat);
        m_ImageViews[i] = m_LogicalDevice.createImageView(viewInfo);
    }
}

void CImagesGenerator::Destroy()
{
	//中身が作成されていないなら解放処理も行わない
	if (m_bCreated == false) return;
	m_bCreated = false;

	// イメージビューの解放
	for (auto& imageView : m_ImageViews)
	{
        m_LogicalDevice.destroyImageView(imageView);
	}

    // イメージの解放
    for (auto& image : m_Images)
    {
        m_LogicalDevice.destroyImage(image);
    }

    // メモリーの解放
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
    imageCreateInfo.format = fomat;                         // イメージのフォーマット
    imageCreateInfo.extent.width = extent.width;            // イメージの幅
    imageCreateInfo.extent.height = extent.height;          // イメージの高さ
    imageCreateInfo.extent.depth = 1;                       // イメージの奥行き
    imageCreateInfo.mipLevels = 1;                          // ミップマップレベル
    imageCreateInfo.arrayLayers = 1;                        // レイヤー数
    imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // サンプル数
    imageCreateInfo.tiling = vk::ImageTiling::eOptimal;     // タイリング方式
    imageCreateInfo.usage = vk::ImageUsageFlagBits::eColorAttachment;   // 使用方法
    imageCreateInfo.sharingMode = vk::SharingMode::eExclusive;          // 共有モード
    imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;        // 初期レイアウト

    return imageCreateInfo;
}

vk::ImageViewCreateInfo CImagesGenerator::CreateImageViewInfo(vk::Image image, vk::Format fomat)
{
    // 画像ビュー作成情報の初期化
    vk::ImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.pNext;
    imageViewCreateInfo.flags;
    imageViewCreateInfo.image = image;                                  // View を作成するための Image
    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;              // Image の種類 (1D, 2D, 3D, Cube など)
    imageViewCreateInfo.format = fomat;                                 // Image データのフォーマット(imageと同じものでなければならない)
    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity; // RGBA コンポーネントを他の RGBA 値にリマップすることができます
    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;

    // Subresource は Image の一部だけを表示するための設定です
    imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;  // Image のどの面を表示するか (例: COLOR_BIT は色を表示するため)
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;          // 表示を開始する Mipmap レベル
    imageViewCreateInfo.subresourceRange.levelCount = 1;            // 表示する Mipmap レベルの数
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;        // 表示を開始する配列レベル
    imageViewCreateInfo.subresourceRange.layerCount = 1;            // 表示する配列レベルの数

    return imageViewCreateInfo;
}

vk::MemoryAllocateInfo CImagesGenerator::AllocateImageMemory(vk::Image image, vk::Device device, vk::PhysicalDevice physicalDevice)
{
    // イメージのメモリ要件を取得
    vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(image);

    // メモリの割り当て情報を設定
    vk::MemoryAllocateInfo allocateInfo;
    allocateInfo.pNext;
    allocateInfo.allocationSize = memoryRequirements.size;  // 画像のメモリサイズ
    allocateInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memoryRequirements.memoryTypeBits);   // メモリタイプ

    return allocateInfo;
}

uint32_t CImagesGenerator::FindMemoryType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter)
{
    // 物理デバイスからメモリプロパティを取得。
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

    // 探しているメモリプロパティのフラグ
    vk::MemoryPropertyFlags properties = vk::MemoryPropertyFlagBits::eDeviceLocal;

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        // typeFilterは、ビットマスクでメモリタイプをフィルタリングするためのもの。
        // ここでは、typeFilterのビットがi番目のメモリタイプに対応するかどうかを確認しています。
        // (1 << i)は、iビット目のビットを1にしたビットマスクです。
        // typeFilter & (1 << i) は、iビット目のビットが立っているかを確認します。
        if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    // エラーメッセージ
    throw std::runtime_error("適切なメモリタイプを見つけられませんでした!");
}
