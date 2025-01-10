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

    // ディスクリプタの作成と更新
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*allocator, &allocatorInfo); // アロケータに入っている情報を間接的に取得
    vk::Device logicalDevice(allocatorInfo.device);
}

vk::Buffer SceneCamera::GetProjectionBuffer()
{
    return m_ProjectionBuffer.GetBuffer();
}

