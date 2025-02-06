#include "DescriptorHandleManeger.h"
#include <iostream>

TextureDescriptorManeger::TextureDescriptorManeger(vk::Device* pLogicalDevice) :
	m_pLogicalDevice(pLogicalDevice),
	m_Descriptors()
{
}

void TextureDescriptorManeger::SetDescriptorSet(std::weak_ptr<vk::DescriptorSetLayout> layout)
{
    // 既に登録済みなら何もしない
    if (HasDescriptor(layout) == true) return;

    // weak_ptr から shared_ptr を取得して、有効であるか確認
    auto spLayout = layout.lock();
    if (!spLayout || *spLayout.get() == VK_NULL_HANDLE) 
    {
        std::cerr << "SetDescriptorSet: layout is expired." << std::endl;
        return;
    }

    // 必要に応じた VDescriptorBase 派生クラスのインスタンスを生成
    VSingleTextureDescriptor pVDescriptor;
    pVDescriptor.Initialize(m_pLogicalDevice, *spLayout.get());
    m_Descriptors.emplace(layout, pVDescriptor);
}

VSingleTextureDescriptor TextureDescriptorManeger::GetVDescriptorSet(std::weak_ptr<vk::DescriptorSetLayout> layout)
{
    CleanupDeathOwner();
    auto it = m_Descriptors.find(layout);
    if (it != m_Descriptors.end()) {
        return it->second;  // コピー（もしくはムーブ）して返す
    }

    return VSingleTextureDescriptor(); // デフォルト値を返す
}

void TextureDescriptorManeger::DeleteDescriptorSet(std::weak_ptr<vk::DescriptorSetLayout> layout)
{
    auto it = m_Descriptors.find(layout);
    if (it != m_Descriptors.end()) {
        m_Descriptors.erase(it);
    }
}

bool TextureDescriptorManeger::HasDescriptor(std::weak_ptr<vk::DescriptorSetLayout> layout)
{
    return (m_Descriptors.find(layout) != m_Descriptors.end());
}

void TextureDescriptorManeger::CleanupDeathOwner()
{
    for (auto it = m_Descriptors.begin(); it != m_Descriptors.end(); ) 
    {
        if (it->first.expired()) {
            it = m_Descriptors.erase(it);
        }
        else {
            ++it;
        }
    }
}

void TextureDescriptorManeger::UpdateAll(vk::ImageView imageView, vk::Sampler sampler)
{
    // 登録されているすべてのディスクリプタセットを更新
    for (auto& pair : m_Descriptors)
    {
        pair.second.Update(imageView, sampler);
    }
}
