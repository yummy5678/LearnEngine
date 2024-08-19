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
	//中身が作成されていないなら解放処理も行わない
	if (m_bCreated == false) return;
	m_bCreated = false;

	//イメージの解放
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
    imageCreateInfo.format = vk::Format::eR8G8B8A8Unorm;    // イメージのフォーマット
    imageCreateInfo.extent.width = extent.width;            // イメージの幅
    imageCreateInfo.extent.height = extent.height;          // イメージの高さ
    imageCreateInfo.extent.depth = extent.depth;            // イメージの深さ
    imageCreateInfo.mipLevels = 1;                          // ミップマップレベル
    imageCreateInfo.arrayLayers = 1;                        // レイヤー数
    imageCreateInfo.samples = vk::SampleCountFlagBits::e1;  // サンプル数
    imageCreateInfo.tiling = vk::ImageTiling::eOptimal;     // タイリング方式
    imageCreateInfo.usage = vk::ImageUsageFlagBits::eColorAttachment;   // 使用方法
    imageCreateInfo.sharingMode = vk::SharingMode::eExclusive;          // 共有モード
    imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;        // 初期レイアウト

    return imageCreateInfo;
}

vk::ImageViewCreateInfo CImagesGenerator::CreateImageViewInfo(vk::Image image)
{
    // 画像ビュー作成情報の初期化
    vk::ImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.image = image;                                            // View を作成するための Image
    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;                         // Image の種類 (1D, 2D, 3D, Cube など)
    imageViewCreateInfo.format = vk::Format::eB8G8R8A8Unorm;                                          // Image データのフォーマット
    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;             // RGBA コンポーネントを他の RGBA 値にリマップすることができます
    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;

    // Subresource は Image の一部だけを表示するための設定です
    imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;  // Image のどの面を表示するか (例: COLOR_BIT は色を表示するため)
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;          // 表示を開始する Mipmap レベル
    imageViewCreateInfo.subresourceRange.levelCount = 1;            // 表示する Mipmap レベルの数
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;        // 表示を開始する配列レベル
    imageViewCreateInfo.subresourceRange.layerCount = 1;            // 表示する配列レベルの数

}

vk::DeviceMemory CImagesGenerator::AllocateImageMemory(vk::Device device, vk::PhysicalDevice physicalDevice, vk::Image image)
{
    // イメージのメモリ要件を取得
    vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(image);

    // メモリの割り当て情報を設定
    vk::MemoryAllocateInfo allocInfo;
    // ホスト側のメモリに置かれ、CPU から読み書きできるメモリを探す
    vk::MemoryPropertyFlags properties = vk::MemoryPropertyFlagBits::eDeviceLocal;
    allocInfo.allocationSize = memoryRequirements.size;  // メモリサイズ
    allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, properties);

    // メモリを確保
    vk::DeviceMemory imageMemory;
    try 
    {
        imageMemory = device.allocateMemory(allocInfo);
    }
    catch (vk::SystemError& err) {
        throw std::runtime_error("イメージメモリの確保に失敗しました!");
    }

    // イメージにメモリをバインド
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

    throw std::runtime_error("適切なメモリタイプを見つけられませんでした!");
}
