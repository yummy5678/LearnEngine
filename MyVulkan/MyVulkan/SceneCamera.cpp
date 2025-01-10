#include "SceneCamera.h"


SceneCamera::SceneCamera() :
    m_ProjectionBuffer()
{
}

SceneCamera::~SceneCamera()
{
}

void SceneCamera::UpdateBuffer(VmaAllocator* allocator, ViewProjection viewProjection)
{
    m_ProjectionBuffer.Update(allocator, viewProjection);

    // �f�B�X�N���v�^�̍쐬�ƍX�V
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*allocator, &allocatorInfo); // �A���P�[�^�ɓ����Ă�������ԐړI�Ɏ擾
    vk::Device logicalDevice(allocatorInfo.device);
}

vk::Buffer SceneCamera::GetProjectionBuffer()
{
    return m_ProjectionBuffer.GetBuffer();
}

