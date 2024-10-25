#include "SceneCamera.h"

SceneCamera::~SceneCamera()
{
}

void SceneCamera::UpdateVBuffer(VmaAllocator allocator, ViewProjection viewProjection)
{
	m_ProjectionBuffer.Update(allocator, viewProjection);
}

vk::DescriptorSet SceneCamera::GetDescriptorSet()
{
	return m_DescriptorSet;
}

void SceneCamera::CreateDescriptorSet()
{
	vk::DescriptorSetLayout layout;

	// Descriptor Set Allocation Info
	VkDescriptorSetAllocateInfo setAllocInfo = {};
	setAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setAllocInfo.descriptorPool = descriptorPool;									// Pool to allocate Descriptor Set from
	setAllocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());// Number of sets to allocate
	setAllocInfo.pSetLayouts = setLayouts.data();									// Layouts to use to allocate sets (1:1 relationship)

	// Allocate descriptor sets (multiple)
	VkResult result = vkAllocateDescriptorSets(mainDevice.logicalDevice, &setAllocInfo, descriptorSets.data());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate Descriptor Sets!");
	}

}
