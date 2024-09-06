#pragma once

#include <stb/stb_image_write.h>

#include <fstream>
#include <vulkan/vulkan.hpp>




static vk::MemoryAllocateInfo GetSuitableMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Image image)
{
    vk::PhysicalDeviceMemoryProperties memProps = physicalDevice.getMemoryProperties();

    vk::MemoryRequirements imgMemReq = logicalDevice.getImageMemoryRequirements(image);

    vk::MemoryAllocateInfo imgMemAllocInfo;
    imgMemAllocInfo.allocationSize = imgMemReq.size;


    for (size_t i = 0; i < memProps.memoryTypeCount; i++)
    {
        if (imgMemReq.memoryTypeBits & (1 << i) &&
            (memProps.memoryTypes[i].propertyFlags & vk::MemoryPropertyFlagBits::eHostVisible))
        {
            imgMemAllocInfo.memoryTypeIndex = i;
            return imgMemAllocInfo;
        }
    }

    printf("使用可能なメモリタイプがありません。");
    return imgMemAllocInfo;
}


static bool WriteVulkanImage(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, const char* imageName, vk::Image image, vk::Extent2D imageSize)
{
    vk::MemoryRequirements imgMemReq = logicalDevice.getImageMemoryRequirements(image);

    vk::MemoryAllocateInfo imgMemAllocInfo = GetSuitableMemoryType(logicalDevice, physicalDevice, image);

    vk::DeviceMemory imgMem = logicalDevice.allocateMemory(imgMemAllocInfo);


    void* imgData = logicalDevice.mapMemory(imgMem, 0, imgMemReq.size);

    int comp = 4;
    int result;
    result = stbi_write_bmp(imageName, imageSize.width, imageSize.height, comp, imgData);

    //解放処理
    logicalDevice.unmapMemory(imgMem);

    return result;
}


static bool WriteVulkanImage(const char* imageName, void* imageData, vk::Extent2D imageSize)
{
    int comp = 4;
    int result;
    result = stbi_write_bmp(imageName, imageSize.width, imageSize.height, comp, imageData);

    return result;
}













