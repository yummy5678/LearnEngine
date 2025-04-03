#include "SceneCamera.h"


SceneCamera::SceneCamera(VmaAllocator* allocator) :
    m_pAllocator(allocator),
    m_LogicalDevice(VK_NULL_HANDLE),
    m_ProjectionBuffer()
{
    if(m_pAllocator == VK_NULL_HANDLE)  std::cerr << "�V�[���J�����̍쐬�Ɏ��s���܂���" << std::endl;
    Initialize();
}

SceneCamera::~SceneCamera()
{
}

void SceneCamera::UpdateBuffer(ViewProjection viewProjection)
{
    m_ProjectionBuffer.SetData(m_pAllocator, viewProjection);

}

vk::Buffer SceneCamera::GetProjectionBuffer()
{
    return m_ProjectionBuffer.GetBuffer();
}

vk::DescriptorSet SceneCamera::GetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
    // �����̃��C�A�E�g�̃f�B�X�N���v�^�Z�b�g�������Ă��Ȃ���΍쐬���Ă���n��
    if (m_Descriptors.find(layout) == m_Descriptors.end())
        SetDescriptorSet(layout);

    return m_Descriptors[layout]->GetDescriptorSet();
}

void SceneCamera::Initialize()
{
    m_ProjectionBuffer.Initialize(m_pAllocator);

    // �f�B�X�N���v�^�̍쐬�ƍX�V
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*m_pAllocator, &allocatorInfo); // �A���P�[�^�ɓ����Ă�������ԐړI�Ɏ擾
    m_LogicalDevice = allocatorInfo.device;
}

void SceneCamera::SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
    // NULL�`�F�b�N
    if (m_LogicalDevice == VK_NULL_HANDLE || *layout.get() == VK_NULL_HANDLE)
    {
        std::cerr << "�J�����̃f�B�X�N�v���^�Z�b�g�̍쐬�Ɏ��s���܂���" << std::endl;
        return;
    }

    // �K�v�ɉ����� VDescriptorBase �h���N���X�̃C���X�^���X�𐶐�
    VSingleBufferDescriptor* descriptor = new VSingleBufferDescriptor;
    descriptor->Initialize(m_LogicalDevice, *layout.get());
    descriptor->Update(m_ProjectionBuffer.GetBuffer(), m_ProjectionBuffer.GetDataSize());
    m_Descriptors.insert({ layout, descriptor });
}

void SceneCamera::DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
    m_Descriptors.erase(layout);
}

void SceneCamera::CleanupDeathOwner()
{
    for (auto& pair : m_Descriptors)
    {
        // shared_ptr�̒��g������������Ă�����폜
        if (*pair.first.get() == VK_NULL_HANDLE) 
        { 
            free(pair.second);
            m_Descriptors.erase(pair.first);
        }
    }
}

void SceneCamera::UpdateAll(vk::Buffer buffer, vk::DeviceSize bufferSize)
{
    // �폜����Ă���f�B�X�N���v�^�Z�b�g���O��
    CleanupDeathOwner();

    // �o�^����Ă��邷�ׂẴf�B�X�N���v�^�Z�b�g���X�V
    for (auto& pair : m_Descriptors)
    {
        pair.second->Update(buffer, bufferSize);
    }
}

