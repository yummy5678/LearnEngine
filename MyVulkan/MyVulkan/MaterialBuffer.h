#pragma once
#include "MeshObject.h"
#include "ImageBuffer.h"
#include "VTextureDescriptor.h"


class VMaterial
{
public:
	VMaterial();
	~VMaterial();

	void SetMaterial(VmaAllocator allocator, Material material);

	vk::Image			GetTextureBuffer();
	vk::DescriptorSet	GetDescriptorSet();
private:
	//glm::vec4 baseColor;	// ñ¢çÏê¨
	//float metallic;
	//float roughness;
	VTextureBuffer	m_Texture;

	vk::Sampler			m_Sampler;
	VTextureDescriptor	m_SamplerDescriptor;

	void SetTexture(VmaAllocator allocator, Texture& texture);
	void CreateSampler(vk::Device logicalDevice);
	void CreateSamplerDescriptor(vk::Device logicalDevice);
};

