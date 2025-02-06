#include "DescriptorHandleManeger.h"
#include <iostream>

TextureDescriptorManeger::TextureDescriptorManeger(vk::Device* pLogicalDevice) :
	m_pLogicalDevice(pLogicalDevice),
	m_Descriptors()
{
}

void TextureDescriptorManeger::SetDescriptorSet(std::weak_ptr<vk::DescriptorSetLayout> layout)
{
    // ���ɓo�^�ς݂Ȃ牽�����Ȃ�
    if (HasDescriptor(layout) == true) return;

    // weak_ptr ���� shared_ptr ���擾���āA�L���ł��邩�m�F
    auto spLayout = layout.lock();
    if (!spLayout || *spLayout.get() == VK_NULL_HANDLE) 
    {
        std::cerr << "SetDescriptorSet: layout is expired." << std::endl;
        return;
    }

    // �K�v�ɉ����� VDescriptorBase �h���N���X�̃C���X�^���X�𐶐�
    VSingleTextureDescriptor pVDescriptor;
    pVDescriptor.Initialize(m_pLogicalDevice, *spLayout.get());
    m_Descriptors.emplace(layout, pVDescriptor);
}

VSingleTextureDescriptor TextureDescriptorManeger::GetVDescriptorSet(std::weak_ptr<vk::DescriptorSetLayout> layout)
{
    CleanupDeathOwner();
    auto it = m_Descriptors.find(layout);
    if (it != m_Descriptors.end()) {
        return it->second;  // �R�s�[�i�������̓��[�u�j���ĕԂ�
    }

    return VSingleTextureDescriptor(); // �f�t�H���g�l��Ԃ�
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
    // �o�^����Ă��邷�ׂẴf�B�X�N���v�^�Z�b�g���X�V
    for (auto& pair : m_Descriptors)
    {
        pair.second.Update(imageView, sampler);
    }
}
