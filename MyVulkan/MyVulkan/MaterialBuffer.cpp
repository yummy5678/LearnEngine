#include "MaterialBuffer.h"

VMaterial::VMaterial() : 
	m_Texture(),
	m_Sampler(VK_NULL_HANDLE)
{
}

VMaterial::~VMaterial()
{
}

void VMaterial::SetMaterial(VmaAllocator* allocator, Material material)
{
	SetTexture(allocator, material.texture);

	if (m_Sampler == VK_NULL_HANDLE)
	{
		VmaAllocatorInfo allocatorInfo;
		vmaGetAllocatorInfo(*allocator, &allocatorInfo);
		CreateSampler(allocatorInfo.device);
	}
}

vk::Image VMaterial::GetTextureBuffer()
{
	return m_Texture.GetImageBuffer();
}

vk::ImageView VMaterial::GetTextureImageView()
{
	return m_Texture.GetImageView();
}


vk::Sampler VMaterial::GetSampler()
{
	return m_Sampler;
}

void VMaterial::SetMMaterialUpdateObserver(ObserveFunction function)
{
	m_BufferUpdateSubject.addObserver(function);
}

void VMaterial::DeleteMaterialUpdateObserver(ObserveFunction function)
{
	m_BufferUpdateSubject.removeObserver(function);
}

void VMaterial::SetTexture(VmaAllocator* allocator, Texture& texture)
{
	m_Texture.SetImage(allocator, texture);
	m_BufferUpdateSubject.notifyObservers();
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
	samplerCreateInfo.maxAnisotropy = 16.0f;
	samplerCreateInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
	samplerCreateInfo.unnormalizedCoordinates = false;
	samplerCreateInfo.compareEnable = false;
	samplerCreateInfo.compareOp = vk::CompareOp::eAlways;
	samplerCreateInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
	samplerCreateInfo.mipLodBias = 0.0f;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = 0.0f;

	m_Sampler = logicalDevice.createSampler(samplerCreateInfo);

}

