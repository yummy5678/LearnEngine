#include "DescriptorHandleManeger.h"


TextureDescriptorManager::TextureDescriptorManager(vk::Device* pLogicalDevice) :
    m_pLogicalDevice(pLogicalDevice),
    m_Descriptors()
{
}

void TextureDescriptorManager::SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
    // ���ɓo�^�ς݂Ȃ牽�����Ȃ�
    if (HasDescriptor(layout)) return;

    if (!layout || *layout == VK_NULL_HANDLE)
    {
        std::cerr << "SetDescriptorSet: layout is invalid." << std::endl;
        return;
    }

    // �K�v�ɉ����� VDescriptorBase �h���N���X�̃C���X�^���X�𐶐�
    VSingleTextureDescriptor pVDescriptor;
    pVDescriptor.Initialize(m_pLogicalDevice, *layout);
    m_Descriptors.try_emplace(layout, std::move(pVDescriptor));
}

VSingleTextureDescriptor TextureDescriptorManager::GetVDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
    CleanupDeathOwner();
    auto it = m_Descriptors.find(layout);
    if (it != m_Descriptors.end()) {
        return it->second;  // �R�s�[�i�������̓��[�u�j���ĕԂ�
    }
    return VSingleTextureDescriptor(); // �f�t�H���g�l��Ԃ�
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
        if (it->first.use_count() == 1) { // ���ɎQ�Ƃ��Ȃ��ꍇ�폜
            it = m_Descriptors.erase(it);
        }
        else {
            ++it;
        }
    }
}

void TextureDescriptorManager::UpdateAll(vk::ImageView imageView, vk::Sampler sampler)
{
    // �o�^����Ă��邷�ׂẴf�B�X�N���v�^�Z�b�g���X�V
    for (auto& pair : m_Descriptors)
    {
        pair.second.Update(imageView, sampler);
    }
}
