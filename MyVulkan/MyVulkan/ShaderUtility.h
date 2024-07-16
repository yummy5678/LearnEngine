#pragma once
#include <vulkan/vulkan.hpp>

namespace ShaderUtility
{
	vk::ShaderModule createShaderModule(const std::vector<char>& code, vk::Device logicalDevice);
};

