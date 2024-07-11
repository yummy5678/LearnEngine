#pragma once
#include <vulkan/vulkan.hpp>

// キューファミリーのインデックス（場所）（存在する場合）
//indicesはindexの複数形
struct QueueFamilyIndices {
    int graphicsFamily = -1;         // グラフィックスキューファミリーの場所
    int presentationFamily = -1;     // プレゼンテーションキューファミリーの場所

    // キューファミリーが有効かどうかをチェックする
    bool isValid()
    {
        return graphicsFamily >= 0 && presentationFamily >= 0;
    }
};

namespace QueueUtility
{
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);


};

