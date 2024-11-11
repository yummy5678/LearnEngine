#include "SwapchainImage.h"

SwapChainImage::SwapChainImage()
{
}

SwapChainImage::~SwapChainImage()
{
}

void SwapChainImage::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
    CreateColor(logicalDevice, swapchain, m_SwapchainInfo);
    CreateDepth(logicalDevice, physicalDevice, m_SwapchainInfo);
}

void SwapChainImage::CreateColor(vk::Device logicalDevice, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
    m_LogicalDevice = logicalDevice;
    m_Size                      = m_SwapchainInfo.minImageCount;
    vk::Extent2D imageExtent    = m_SwapchainInfo.imageExtent;

    // 注:スワップチェインの画像はgetSwapchainImagesKHRから
    // 取得したものを使用しなければならない。
    m_ColorImages = logicalDevice.getSwapchainImagesKHR(swapchain);

    //m_ImageMemory.resize(m_Size);
    m_ColorImageViews.resize(m_Size);

    std::vector<vk::ImageViewCreateInfo> viewInfo;
    viewInfo.resize(m_Size);

    vk::Format fomat = m_SwapchainInfo.imageFormat;
    for (uint32_t i = 0; i < m_Size; i++)
    {
        //画像を扱う際の情報を設定
        auto viewInfo = CreateImageViewInfo(m_ColorImages[i], fomat, vk::ImageAspectFlagBits::eColor);
        m_ColorImageViews[i] = m_LogicalDevice.createImageView(viewInfo);
    }
}

void SwapChainImage::CreateDepth(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
    m_LogicalDevice = logicalDevice;
    m_Size = m_SwapchainInfo.minImageCount;

    m_DepthImages.resize(m_Size);
    m_DepthImageMemory.resize(m_Size);
    m_DepthImageViews.resize(m_Size);


    std::vector<vk::ImageViewCreateInfo> viewInfo;
    viewInfo.resize(m_Size);

    
    vk::Extent2D imageExtent = m_SwapchainInfo.imageExtent;
    vk::Format fomat = m_SwapchainInfo.imageFormat;
    auto imageInfo = CreateImageInfo(imageExtent, fomat, vk::ImageUsageFlagBits::eDepthStencilAttachment);
    for (uint32_t i = 0; i < m_Size; i++)
    {
        //画像を作成
        m_DepthImages[i] = m_LogicalDevice.createImage(imageInfo);

        //画像のメモリを確保
        auto allocInfo = AllocateImageMemory(logicalDevice, physicalDevice, m_DepthImages[i], vk::MemoryPropertyFlagBits::eDeviceLocal);
        m_DepthImageMemory[i] = m_LogicalDevice.allocateMemory(allocInfo);
        m_LogicalDevice.bindImageMemory(m_DepthImages[i], m_DepthImageMemory[i], 0);  // バインド

        //画像を扱う際の情報を設定
        auto viewInfo = CreateImageViewInfo(m_DepthImages[i], fomat, vk::ImageAspectFlagBits::eDepth);
        m_DepthImageViews[i] = m_LogicalDevice.createImageView(viewInfo);
    }
}

void SwapChainImage::Destroy()
{
	// イメージビューの解放
	for (auto& imageView : m_ColorImageViews)
	{
        m_LogicalDevice.destroyImageView(imageView);
	}

    // イメージの解放
    for (auto& image : m_ColorImages)
    {
        m_LogicalDevice.destroyImage(image);
    }

    // メモリーの解放
    for (auto& memory : m_ColorImageMemory)
    {
        m_LogicalDevice.freeMemory(memory);
    }
}

std::vector<vk::Image> SwapChainImage::GetColorImages()
{
	return m_ColorImages;
}

std::vector<vk::ImageView> SwapChainImage::GetColorImageViews()
{
    return m_ColorImageViews;
}

uint32_t SwapChainImage::GetSize()
{
    return m_Size;
}

vk::ImageCreateInfo SwapChainImage::CreateImageInfo(vk::Extent2D extent, vk::Format fomat,vk::ImageUsageFlags usage)
{
    vk::ImageCreateInfo imageCreateInfo;
    imageCreateInfo.pNext;
    imageCreateInfo.flags;
    imageCreateInfo.imageType = vk::ImageType::e2D;
    imageCreateInfo.format = fomat;                                 // 画像のフォーマット
    imageCreateInfo.extent.width = extent.width;                    // 画像の幅
    imageCreateInfo.extent.height = extent.height;                  // 画像の高さ
    imageCreateInfo.extent.depth = 1;                               // 画像の奥行き
    imageCreateInfo.mipLevels = 1;                                  // ミップマップレベル
    imageCreateInfo.arrayLayers = 1;                                // レイヤー数
    imageCreateInfo.samples = vk::SampleCountFlagBits::e1;          // サンプル数
    imageCreateInfo.tiling = vk::ImageTiling::eOptimal;             // タイリング方式
    imageCreateInfo.usage = usage;                                  // 使用方法
    imageCreateInfo.sharingMode = vk::SharingMode::eExclusive;      // 共有モード
    imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;    // 初期レイアウト

    return imageCreateInfo;
}

vk::ImageViewCreateInfo SwapChainImage::CreateImageViewInfo(vk::Image image, vk::Format fomat, vk::ImageAspectFlags aspectFlag)
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
    //VK_IMAGE_ASPECT_COLOR_BIT
    imageViewCreateInfo.subresourceRange.aspectMask = aspectFlag;   // Image のどの面を表示するか (例: COLOR_BIT は色を表示するため)
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;          // 表示を開始する Mipmap レベル
    imageViewCreateInfo.subresourceRange.levelCount = 1;            // 表示する Mipmap レベルの数
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;        // 表示を開始する配列レベル
    imageViewCreateInfo.subresourceRange.layerCount = 1;            // 表示する配列レベルの数

    return imageViewCreateInfo;
}

vk::MemoryAllocateInfo SwapChainImage::AllocateImageMemory(vk::Device device, vk::PhysicalDevice physicalDevice, vk::Image image, vk::MemoryPropertyFlags propertyFlags)
{
    // イメージのメモリ要件を取得
    vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(image);

    // 探しているメモリプロパティのフラグ
    vk::MemoryPropertyFlags properties = propertyFlags;
    //vk::MemoryPropertyFlags properties = vk::MemoryProperty::MemoryPropertyFlagBits::eDeviceLocalFlagBits::eHostVisible;

    // メモリの割り当て情報を設定
    vk::MemoryAllocateInfo allocateInfo;
    allocateInfo.pNext;
    allocateInfo.allocationSize = memoryRequirements.size;  // 画像のメモリサイズ
    allocateInfo.memoryTypeIndex = FindMemoryType(device, physicalDevice, image, properties);   // メモリタイプ

    return allocateInfo;
}

uint32_t SwapChainImage::FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Image image, vk::MemoryPropertyFlags findType)
{
    // イメージのメモリ要件を取得
    vk::MemoryRequirements memoryRequirements = logicalDevice.getImageMemoryRequirements(image);
    auto typeFilter = memoryRequirements.memoryTypeBits;

    // 物理デバイス(GPU)からメモリプロパティを取得。
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        // typeFilterは、ビットマスクでメモリタイプをフィルタリングするためのもの。
        // ここでは、typeFilterのビットがi番目のメモリタイプに対応するかどうかを確認しています。
        // (1 << i)は、iビット目のビットを1にしたビットマスクです。
        // typeFilter & (1 << i) は、iビット目のビットが立っているかを確認します。
        if ((typeFilter & (1 << i)) && 
            (memoryProperties.memoryTypes[i].propertyFlags & findType) == findType)
        {
            return i;
        }
    }

    // エラーメッセージ
    throw std::runtime_error("適切なメモリタイプを見つけられませんでした!");
}
