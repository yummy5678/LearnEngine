#include "FramebufferGenerator.h"


FramebufferGenerator::FramebufferGenerator()
{
    m_ClassName = "FramebufferGenerator";
}

FramebufferGenerator::~FramebufferGenerator()
{

}

void FramebufferGenerator::Create(vk::Device logicalDevice, std::vector<vk::ImageView> imageViews, vk::RenderPass renderPass, vk::Extent2D extent)
{
    m_bCreated = true;

    m_LogicalDevice = logicalDevice;

    auto attachments = CreateAttachments(imageViews);

    //フレームバッファ作成情報を作成
    m_FramebufferInfos = CreateFramebufferInfos(attachments, renderPass, extent);

    //作成情報からフレームバッファを作成
    for (const auto& info : m_FramebufferInfos)
    {
        m_Framebuffers.push_back(logicalDevice.createFramebuffer(info));
    }
}

void FramebufferGenerator::Destroy()
{
    //中身が作成されていないなら解放処理も行わない
    if (m_bCreated == false) return;
    m_bCreated = false;

    // フレームバッファの解放
    for (const auto& framebuffer : m_Framebuffers)
        m_LogicalDevice.destroyFramebuffer(framebuffer);
}

std::vector<std::vector<vk::ImageView>> FramebufferGenerator::CreateAttachments(std::vector<vk::ImageView> imageViews)
{
    std::vector<std::vector<vk::ImageView>> attachments;
    attachments.reserve(imageViews.size());

    for (auto &imageView : imageViews)
    {
        //1つのフレームに保存する画像の種類
        std::vector<vk::ImageView> attachment = { imageView };   //ここでは最終出力の色データのみ  
        attachments.push_back(attachment);
    }

    return attachments;
}

std::vector<vk::Framebuffer> FramebufferGenerator::GetFramebuffers()
{
    CheckCreated();
    return m_Framebuffers;
}

std::vector<vk::FramebufferCreateInfo> FramebufferGenerator::CreateFramebufferInfos(std::vector<std::vector<vk::ImageView>>& attachments, vk::RenderPass renderPass, vk::Extent2D extent)
{
    std::vector<vk::FramebufferCreateInfo> framebufferInfos;
    framebufferInfos.resize(attachments.size());

    for (int i = 0;i < framebufferInfos.size();i++)
    {
        vk::FramebufferCreateInfo createInfo;
        createInfo.pNext;
        createInfo.flags;
        createInfo.renderPass       = renderPass;                       //依存しているレンダーパス
        createInfo.attachmentCount  = (uint32_t)attachments[i].size();  //画像の種類数
        createInfo.pAttachments     = attachments[i].data();            //画像のデータ
        createInfo.width            = extent.width;                     //画像の幅
        createInfo.height           = extent.height;                    //画像の高さ
        createInfo.layers           = 1;                                //視点の数

        // vk::Framebufferをvk::UniqueFramebufferに変換する
        framebufferInfos[i] = createInfo;
    }

    return framebufferInfos;
}


