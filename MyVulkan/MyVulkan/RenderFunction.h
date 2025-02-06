#pragma once
#include <vulkan/vulkan.h>
#include <functional>


using RenderFunction = std::function<void(vk::CommandBuffer commandBuffer, void* key)>;