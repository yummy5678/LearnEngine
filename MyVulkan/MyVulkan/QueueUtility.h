#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "GeneratorBase.h"

//キューを作成するときの初期値
constexpr int Number_NoneQueue = -1;

class QueueFamilyGenerator : CGeneratorBase
{
public:
    QueueFamilyGenerator();
    ~QueueFamilyGenerator();

    void Create(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

    vk::Queue GetGraphics(vk::Device logicalDevice);
    vk::Queue GetPresentation(vk::Device logicalDevice);
    std::vector<vk::DeviceQueueCreateInfo> GetQueueInfos();

private:   
    int m_GraphicsFamilyIndex = Number_NoneQueue;      // グラフィックスキューファミリーの場所
    int m_PresentationFamilyIndex = Number_NoneQueue;  // プレゼンテーションキューファミリー(画像の表示)の場所

    // キュー作成情報用のベクター
    std::vector<vk::DeviceQueueCreateInfo> m_QueueCreateInfos;

    int SearchGraphicsFamily(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
    int SearchPresentationFamily(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

    //計算要求を受け付けるキューを受け取る
    std::vector<vk::DeviceQueueCreateInfo> CreateQueueInfos(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

};


//以下Queueに関するメモ
//次の Vulkan オブジェクトは、キューがそのオブジェクトを使用するコマンドを実行している間は破棄してはなりません。
//VkFence
//VkSemaphore
//VkCommandBuffer
//VkCommandPool