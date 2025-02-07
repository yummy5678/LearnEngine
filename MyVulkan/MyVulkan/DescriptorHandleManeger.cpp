#include "DescriptorHandleManeger.h"


TextureDescriptorManager::TextureDescriptorManager(vk::Device* pLogicalDevice) :
    m_pLogicalDevice(pLogicalDevice),
    m_Descriptors()
{
}

void TextureDescriptorManager::SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
    // 既に登録済みなら何もしない
    if (HasDescriptor(layout)) return;

    if (!layout || *layout == VK_NULL_HANDLE)
    {
        std::cerr << "SetDescriptorSet: layout is invalid." << std::endl;
        return;
    }

    // 必要に応じた VDescriptorBase 派生クラスのインスタンスを生成
    VSingleTextureDescriptor pVDescriptor;
    pVDescriptor.Initialize(m_pLogicalDevice, *layout);
    m_Descriptors.try_emplace(layout, std::move(pVDescriptor));
}

VSingleTextureDescriptor TextureDescriptorManager::GetVDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
    CleanupDeathOwner();
    auto it = m_Descriptors.find(layout);
    if (it != m_Descriptors.end()) {
        return it->second;  // コピー（もしくはムーブ）して返す
    }
    return VSingleTextureDescriptor(); // デフォルト値を返す
}

void TextureDescriptorManager::DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
    m_Descriptors.erase(layout);
}

bool TextureDescriptorManager::HasDescriptor(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
    return m_Descriptors.find(layout) != m_Descriptors.end();
}

void TextureDescriptorManager::CleanupDeathOwner()
{
    for (auto it = m_Descriptors.begin(); it != m_Descriptors.end(); )
    {
        if (it->first.use_count() == 1) { // 他に参照がない場合削除
            it = m_Descriptors.erase(it);
        }
        else {
            ++it;
        }
    }
}

void TextureDescriptorManager::UpdateAll(vk::ImageView imageView, vk::Sampler sampler)
{
    // 登録されているすべてのディスクリプタセットを更新
    for (auto& pair : m_Descriptors)
    {
        pair.second.Update(imageView, sampler);
    }
}
