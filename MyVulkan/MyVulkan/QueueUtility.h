#pragma once
#include <vulkan/vulkan.hpp>
#include <set>


// キューファミリーのインデックス（場所）（存在する場合）
//indicesはindexの複数形
struct QueueFamilyIndices {
    int graphicsFamily      = -1;   // グラフィックスキューファミリーの場所
    int presentationFamily  = -1;   // プレゼンテーションキューファミリーの場所

    // キューファミリーが有効かどうかをチェックする
    bool isValid()
    {
        return graphicsFamily >= 0 && presentationFamily >= 0;
    }
};

namespace VulkanCreate
{
    //計算要求を受け付けるキューを受け取る
    std::vector< vk::DeviceQueueCreateInfo > GetQueueInfos(vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface);
}

namespace VulkanUtility
{
	QueueFamilyIndices GetQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface);
}


//以下Queueに関するメモ
//次の Vulkan オブジェクトは、キューがそのオブジェクトを使用するコマンドを実行している間は破棄してはなりません。
//VkFence
//VkSemaphore
//VkCommandBuffer
//VkCommandPool