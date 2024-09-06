#include "ImagesGenerator.h"

ImagesGenerator::ImagesGenerator()
{
	m_ClassName = "ImagesGenerator";
}

ImagesGenerator::~ImagesGenerator()
{
}


void ImagesGenerator::Create(uint32_t ImageNum, vk::Extent2D extent, vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
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

void ImagesGenerator::CreateForSwapchain(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
    m_bCreated = true;
    m_LogicalDevice = logicalDevice;

    m_Size                      = m_SwapchainInfo.minImageCount;
    m_Fomat                     = m_SwapchainInfo.imageFormat;
    vk::Extent2D imageExtent    = m_SwapchainInfo.imageExtent;

    m_Images.resize(m_Size);
    m_ImageMemory.resize(m_Size);
    m_ImageViews.resize(m_Size);

    std::vector<vk::ImageViewCreateInfo> viewInfo;
    viewInfo.resize(m_Size);


    m_ImageInfo = CreateImageInfo(imageExtent, m_Fomat, vk::ImageLayout::eUndefined);
    for (uint32_t i = 0; i < m_Size; i++)
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

void ImagesGenerator::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t imageCount, vk::Format fomat, vk::Extent2D extent)
{
    m_bCreated = true;
    m_LogicalDevice = logicalDevice;

    m_Fomat = fomat;

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

void ImagesGenerator::Destroy()
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

std::vector<vk::Image> ImagesGenerator::GetImages()
{
	CheckCreated();
	return m_Images;
}

std::vector<vk::ImageView> ImagesGenerator::GetImageViews()
{
    CheckCreated();
    return m_ImageViews;
}


// 画像データをCPU側から読み取れるデータに展開する
std::vector<void*> ImagesGenerator::GetImageData()
{
    std::vector<void*> imageData;
    imageData.resize(m_ImageMemory.size());

    for (int i = 0;i < m_ImageMemory.size();i++)
    {
        vk::MemoryRequirements imgMemReq = m_LogicalDevice.getImageMemoryRequirements(m_Images[i]);
        imageData[i] = m_LogicalDevice.mapMemory(m_ImageMemory[i], 0, imgMemReq.size);
    }


    return imageData;
}

vk::ImageCreateInfo ImagesGenerator::GetImageInfo()
{
    CheckCreated();
    return m_ImageInfo;
}

vk::Format ImagesGenerator::GetFomat()
{
    CheckCreated();
    return m_Fomat;
}

uint32_t ImagesGenerator::GetSize()
{
    return m_Size;
}


vk::ImageCreateInfo ImagesGenerator::CreateImageInfo(vk::Extent2D extent, vk::Format fomat)
{
    vk::ImageCreateInfo imageCreateInfo;
    imageCreateInfo.pNext;
    imageCreateInfo.flags;
    imageCreateInfo.imageType = vk::ImageType::e2D;
    imageCreateInfo.format = fomat;                         // 画像のフォーマット
    imageCreateInfo.extent.width = extent.width;            // 画像の幅
    imageCreateInfo.extent.height = extent.height;          // 画像の高さ
    imageCreateInfo.extent.depth = 1;                       // 画像の奥行き
    imageCreateInfo.mipLevels = 1;                          // ミップマップレベル
    imageCreateInfo.arrayLayers = 1;                        // レイヤー数
    imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // サンプル数
    imageCreateInfo.tiling = vk::ImageTiling::eOptimal;     // タイリング方式
    imageCreateInfo.usage = vk::ImageUsageFlagBits::eColorAttachment;   // 使用方法
    imageCreateInfo.sharingMode = vk::SharingMode::eExclusive;          // 共有モード
    imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;        // 初期レイアウト

    return imageCreateInfo;
}

vk::ImageCreateInfo ImagesGenerator::CreateImageInfo(vk::Extent2D extent, vk::Format fomat, vk::ImageLayout layout)
{
    vk::ImageCreateInfo imageCreateInfo;
    imageCreateInfo.pNext;
    imageCreateInfo.flags;
    imageCreateInfo.imageType = vk::ImageType::e2D;
    imageCreateInfo.format = fomat;                         // 画像のフォーマット
    imageCreateInfo.extent.width = extent.width;            // 画像の幅
    imageCreateInfo.extent.height = extent.height;          // 画像の高さ
    imageCreateInfo.extent.depth = 1;                       // 画像の奥行き
    imageCreateInfo.mipLevels = 1;                          // ミップマップレベル
    imageCreateInfo.arrayLayers = 1;                        // レイヤー数
    imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // サンプル数
    imageCreateInfo.tiling = vk::ImageTiling::eOptimal;     // タイリング方式
    imageCreateInfo.usage = vk::ImageUsageFlagBits::eColorAttachment;   // 使用方法
    imageCreateInfo.sharingMode = vk::SharingMode::eExclusive;          // 共有モード
    imageCreateInfo.initialLayout = layout;        // 初期レイアウト

    return imageCreateInfo;
}

vk::ImageViewCreateInfo ImagesGenerator::CreateImageViewInfo(vk::Image image, vk::Format fomat)
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

vk::MemoryAllocateInfo ImagesGenerator::AllocateImageMemory(vk::Image image, vk::Device device, vk::PhysicalDevice physicalDevice)
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

uint32_t ImagesGenerator::FindMemoryType(vk::PhysicalDevice physicalDevice, uint32_t typeFilter)
{
    // 物理デバイス(GPU)からメモリプロパティを取得。
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

    // 探しているメモリプロパティのフラグ
    //vk::MemoryPropertyFlags properties = vk::MemoryPropertyFlagBits::eDeviceLocal;
    vk::MemoryPropertyFlags properties = vk::MemoryPropertyFlagBits::eHostVisible;

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
