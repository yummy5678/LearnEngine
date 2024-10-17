#include "MaterialBuffer.h"

VMaterial::VMaterial()
{
}

VMaterial::~VMaterial()
{
}

void VMaterial::SetMaterial(VmaAllocator allocator, Material material)
{


}

vk::Image VMaterial::GetTextureBuffer()
{
	return m_Texture.GetImageBuffer();
}

void VMaterial::SetTexture(VmaAllocator allocator, Texture& texture)
{
	m_Texture.SetImage(allocator, texture);
}

void VMaterial::CreateSampler(vk::Device logicalDevice)
{
	vk::SamplerCreateInfo samplerCreateInfo;
	samplerCreateInfo.magFilter = vk::Filter::eLinear;
	samplerCreateInfo.minFilter = vk::Filter::eLinear;
	samplerCreateInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
	samplerCreateInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
	samplerCreateInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
	samplerCreateInfo.anisotropyEnable = false;
	samplerCreateInfo.maxAnisotropy = 1.0f;
	samplerCreateInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
	samplerCreateInfo.unnormalizedCoordinates = false;
	samplerCreateInfo.compareEnable = false;
	samplerCreateInfo.compareOp = vk::CompareOp::eAlways;
	samplerCreateInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
	samplerCreateInfo.mipLodBias = 0.0f;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = 0.0f;

	try
	{
		m_Sampler = logicalDevice.createSampler(samplerCreateInfo);
	}
	catch (const std::exception&)
	{
		throw std::runtime_error("サンプラーの作成に失敗しました!");
	}
}

void VMaterial::CreateSamplerDescriptor(vk::Device logicalDevice)
{
	m_SamplerDescriptor.GetDescriptorSetLayout();

	vk::DescriptorSetLayoutBinding descSetLayoutBinding; // 変更
	descSetLayoutBinding.binding = 1;
	descSetLayoutBinding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	descSetLayoutBinding.descriptorCount = 1;
	descSetLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;

	vk::DescriptorSetLayoutCreateInfo descSetLayoutCreateInfo{};
	descSetLayoutCreateInfo.bindingCount = std::size(descSetLayoutBinding); // 変更
	descSetLayoutCreateInfo.pBindings = &descSetLayoutBinding;

	vk::DescriptorSetLayout descSetLayout = logicalDevice.createDescriptorSetLayout(descSetLayoutCreateInfo);

	vk::DescriptorPoolSize descPoolSize; // 変更
	descPoolSize.type = vk::DescriptorType::eCombinedImageSampler;
	descPoolSize.descriptorCount = 1;

	vk::DescriptorPoolCreateInfo descPoolCreateInfo;
	descPoolCreateInfo.poolSizeCount = std::size(descPoolSize); // 変更
	descPoolCreateInfo.pPoolSizes = descPoolSize;
	descPoolCreateInfo.maxSets = 1;

	vk::DescriptorPool descPool = logicalDevice.createDescriptorPool(descPoolCreateInfo);

}
