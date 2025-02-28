#include "SwapchainImage.h"


SwapChainImage::SwapChainImage() :
    m_ColorImages(),
    m_ColorFormat(vk::Format::eUndefined),
    m_ColorImageViews(),
    m_DepthImages(),
    m_DepthFormat(vk::Format::eUndefined)



{
}

SwapChainImage::~SwapChainImage()
{
}

void SwapChainImage::Create(VmaAllocator* allocator, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
    // VMAMに紐づけられているオブジェクトの情報を取得
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*allocator, &allocatorInfo);
    m_Size = m_SwapchainInfo.minImageCount;
    try
    {
        CreateColor(allocatorInfo.device, swapchain, m_SwapchainInfo);
        CreateDepth(allocator, m_SwapchainInfo);
    }
    catch (const std::runtime_error& e)
    {
        //エラーメッセージ受け取り
        printf("エラー: %s\n", e.what());
    }


    //m_ColorDescriptor.CreateSingleDescriptorSet();

}

void SwapChainImage::CreateColor(vk::Device logicalDevice, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
	uint32_t imageCount = m_SwapchainInfo.minImageCount;
    vk::Extent2D imageExtent    = m_SwapchainInfo.imageExtent;

    // 注:スワップチェインの画像はgetSwapchainImagesKHRから
    // 取得したものを使用しなければならない。
    m_ColorImages = logicalDevice.getSwapchainImagesKHR(swapchain);

   
    m_ColorImageViews.resize(imageCount);

    std::vector<vk::ImageViewCreateInfo> viewInfo;
    viewInfo.resize(imageCount);

    m_ColorFormat = m_SwapchainInfo.imageFormat;
    for (uint32_t i = 0; i < imageCount; i++)
    {
        //画像を扱う際の情報を設定
        auto viewInfo = CreateImageViewInfo(m_ColorImages[i], m_ColorFormat, vk::ImageAspectFlagBits::eColor);
        m_ColorImageViews[i] = logicalDevice.createImageView(viewInfo);
    }
}

void SwapChainImage::CreateDepth(VmaAllocator* allocator, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
    uint32_t imageCount = m_SwapchainInfo.minImageCount;
    m_DepthImages.resize(imageCount);
    
    vk::Extent2D imageExtent = m_SwapchainInfo.imageExtent;
    m_DepthFormat = vk::Format::eD24UnormS8Uint;
    vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
    auto imageInfo = CreateImageInfo(imageExtent, m_DepthFormat, usage);
    for (uint32_t i = 0; i < imageCount; i++)
    {
        // 画像を作成
		m_DepthImages[i].Create(allocator, imageInfo, vk::ImageAspectFlagBits::eDepth);

        //画像を扱う際の情報を設定
        //auto viewInfo = CreateImageViewInfo(m_DepthImages[i], m_DepthFormat, vk::ImageAspectFlagBits::eDepth);
        //m_DepthImageViews[i] = m_LogicalDevice.createImageView(viewInfo);
    }
}

void SwapChainImage::Destroy()
{
    // VMAMに紐づけられているオブジェクトの情報を取得
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*m_pAllocator, &allocatorInfo);

    vk::Device logicalDevice = allocatorInfo.device;

    // イメージビューの解放
    for (auto& imageView : m_ColorImageViews)
    {
        logicalDevice.destroyImageView(imageView);
    }
}

std::vector<vk::Image> SwapChainImage::GetColorImages()
{
	return m_ColorImages;
}

vk::Format SwapChainImage::GetColorFormat()
{
    return m_ColorFormat;
}

std::vector<vk::ImageView> SwapChainImage::GetColorImageViews()
{
    return m_ColorImageViews;
}


std::vector<VImage> SwapChainImage::GetDepthImages()
{
    return m_DepthImages;
}

vk::Format SwapChainImage::GetDepthFormat()
{
    return m_DepthFormat;
}

std::vector<RenderingImageSet> SwapChainImage::GetImageSets()
{
    std::vector<RenderingImageSet> imageViewSets;
    imageViewSets.resize(m_Size);

    for (uint32_t i = 0; i < m_Size; i++)
    {
        imageViewSets[i].color = m_ColorImageViews[i];
        imageViewSets[i].depth = m_DepthImages[i].GetImageView();
    }


    return imageViewSets;
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
	// imageCreateInfo.queueFamilyIndexCount = 0;                     // キューファミリーインデックスの数
	// imageCreateInfo.pQueueFamilyIndices;                           // キューファミリーインデックスの配列
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
