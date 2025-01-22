#include "SceneCamera.h"


SceneCamera::SceneCamera(VmaAllocator* allocator) :
    m_pAllocator(allocator),
    m_ProjectionBuffer()
{
    Initialize();
}

SceneCamera::~SceneCamera()
{
}

void SceneCamera::UpdateBuffer(ViewProjection viewProjection)
{
    m_ProjectionBuffer.SetData(m_pAllocator, viewProjection);

    // �f�B�X�N���v�^�̍쐬�ƍX�V
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*m_pAllocator, &allocatorInfo); // �A���P�[�^�ɓ����Ă�������ԐړI�Ɏ擾
    vk::Device logicalDevice(allocatorInfo.device);
}

vk::Buffer SceneCamera::GetProjectionBuffer()
{
    return m_ProjectionBuffer.GetBuffer();
}

void SceneCamera::Initialize()
{
    m_ProjectionBuffer.Initialize(m_pAllocator);
}

