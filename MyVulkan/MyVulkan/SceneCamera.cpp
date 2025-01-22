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

    // ディスクリプタの作成と更新
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*m_pAllocator, &allocatorInfo); // アロケータに入っている情報を間接的に取得
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

