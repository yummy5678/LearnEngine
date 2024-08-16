#pragma once
#include <vulkan/vulkan.hpp>
#include <set>

//キューを作成するときの初期値
constexpr int Number_NoneQueue = -1;

struct QueueContainer
{

};

class QueueFamilySelector
{
public:
    QueueFamilySelector(vk::PhysicalDevice physicalDevice);
    ~QueueFamilySelector();

    int GetGraphicIndex();
    int GetComputeIndex();
    int GetTransferIndex();
    int GetPresentationIndex(vk::SurfaceKHR surface);
    std::vector<vk::DeviceQueueCreateInfo> GetQueueInfos(); //デバイスの作成用

private:   
    vk::PhysicalDevice m_PhysicalDevice;
    int m_GraphicsFamilyIndex       = Number_NoneQueue;  // 描画用キューファミリーの場所
    int m_ComputeFamilyIndex        = Number_NoneQueue;  // 計算用キューファミリーの場所
    int m_TransferFamilyIndex       = Number_NoneQueue;  // 転送用キューファミリーの場所
    int m_PresentationFamilyIndex   = Number_NoneQueue;  // プレゼンテーションキューファミリー(画像の表示)の場所

    // キュー作成情報用のベクター
    std::vector<vk::DeviceQueueCreateInfo> m_QueueCreateInfos;

    int SearchGraphicsFamily();
    int SearchComputeFamily();
    int SearchTransferFamily();
    int SearchPresentationFamily(vk::SurfaceKHR surface);

    //計算要求を受け付けるキューを受け取る
   // std::vector<vk::DeviceQueueCreateInfo> CreateQueueInfos();

};


//以下Queueに関するメモ
//次の Vulkan オブジェクトは、キューがそのオブジェクトを使用するコマンドを実行している間は破棄してはなりません。
//VkFence
//VkSemaphore
//VkCommandBuffer
//VkCommandPool