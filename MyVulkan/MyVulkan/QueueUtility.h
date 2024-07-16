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

namespace QueueUtility
{
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    //計算要求を受け付けるキューを受け取る
    std::vector< vk::DeviceQueueCreateInfo > getQueueInfos(vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface);
};

