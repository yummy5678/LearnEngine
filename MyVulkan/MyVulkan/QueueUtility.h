#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>

//キューを作成するときの初期値
constexpr uint32_t UndefinedQueueNumber = UINT32_MAX;   //キューが見つかったかの判定に使う値

class QueueFamilySelector
{
public:
    QueueFamilySelector(vk::PhysicalDevice* pPhysicalDevice);
    ~QueueFamilySelector();

    uint32_t GetGraphicIndex();
    uint32_t GetComputeIndex();
    uint32_t GetTransferIndex();
    uint32_t GetPresentationIndex(vk::SurfaceKHR surface);
    std::vector<vk::DeviceQueueCreateInfo> GetQueueInfos(); //デバイスの作成用

private:   
    vk::PhysicalDevice* m_pPhysicalDevice;
    uint32_t m_GraphicsFamilyIndex       = UndefinedQueueNumber;  // 描画用キューファミリーの場所
    uint32_t m_ComputeFamilyIndex        = UndefinedQueueNumber;  // 計算用キューファミリーの場所
    uint32_t m_TransferFamilyIndex       = UndefinedQueueNumber;  // 転送用キューファミリーの場所
    uint32_t m_PresentationFamilyIndex   = UndefinedQueueNumber;  // プレゼンテーションキューファミリー(画像の表示)の場所

    // キュー作成情報用のベクター
    std::vector<vk::DeviceQueueCreateInfo> m_QueueCreateInfos;

    uint32_t SearchGraphicsFamily();
    uint32_t SearchComputeFamily();
    uint32_t SearchTransferFamily();
    uint32_t SearchPresentationFamily(vk::SurfaceKHR surface);

    //計算要求を受け付けるキューを受け取る
   // std::vector<vk::DeviceQueueCreateInfo> CreateQueueInfos();

};
