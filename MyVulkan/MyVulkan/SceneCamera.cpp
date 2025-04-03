#include "SceneCamera.h"


SceneCamera::SceneCamera(VmaAllocator* allocator) :
    m_pAllocator(allocator),
    m_LogicalDevice(VK_NULL_HANDLE),
    m_ProjectionBuffer()
{
    if(m_pAllocator == VK_NULL_HANDLE)  std::cerr << "シーンカメラの作成に失敗しました" << std::endl;
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
    // 引数のレイアウトのディスクリプタセットを持っていなければ作成してから渡す
    if (m_Descriptors.find(layout) == m_Descriptors.end())
        SetDescriptorSet(layout);

    return m_Descriptors[layout]->GetDescriptorSet();
}

void SceneCamera::Initialize()
{
    m_ProjectionBuffer.Initialize(m_pAllocator);

    // ディスクリプタの作成と更新
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*m_pAllocator, &allocatorInfo); // アロケータに入っている情報を間接的に取得
    m_LogicalDevice = allocatorInfo.device;
}

void SceneCamera::SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
    // NULLチェック
    if (m_LogicalDevice == VK_NULL_HANDLE || *layout.get() == VK_NULL_HANDLE)
    {
        std::cerr << "カメラのディスクプリタセットの作成に失敗しました" << std::endl;
        return;
    }

    // 必要に応じた VDescriptorBase 派生クラスのインスタンスを生成
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
        // shared_ptrの中身が初期化されていたら削除
        if (*pair.first.get() == VK_NULL_HANDLE) 
        { 
            free(pair.second);
            m_Descriptors.erase(pair.first);
        }
    }
}

void SceneCamera::UpdateAll(vk::Buffer buffer, vk::DeviceSize bufferSize)
{
    // 削除されているディスクリプタセットを外す
    CleanupDeathOwner();

    // 登録されているすべてのディスクリプタセットを更新
    for (auto& pair : m_Descriptors)
    {
        pair.second->Update(buffer, bufferSize);
    }
}

