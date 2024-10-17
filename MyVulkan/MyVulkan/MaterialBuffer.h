#pragma once
#include "MeshObject.h"
#include "ImageBuffer.h"
#include "DescriptorHandler.h"


class VMaterial
{
public:
	VMaterial();
	~VMaterial();

	void SetMaterial(VmaAllocator allocator, Material material);

	vk::Image GetTextureBuffer();
	vk::Sampler GetSampler();
private:
	//glm::vec4 baseColor;	// ñ¢çÏê¨
	//float metallic;
	//float roughness;
	VImageBuffer	m_Texture;

	vk::Sampler		m_Sampler;
	VDescriptor		m_SamplerDescriptor;

	void SetTexture(VmaAllocator allocator, Texture& texture);
	void CreateSampler(vk::Device logicalDevice);
	void CreateSamplerDescriptor(vk::Device logicalDevice);
};

