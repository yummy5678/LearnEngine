#pragma once
#include "MeshObject.h"
#include "ImageBuffer.h"


class VMaterial
{
public:
	VMaterial();
	~VMaterial();

	void SetMaterial(VmaAllocator allocator, Material material);

	vk::Image GetTextureBuffer();

private:
	//glm::vec4 baseColor;	// ñ¢çÏê¨
	//float metallic;
	//float roughness;
	VImageBuffer	m_Texture;


	void SetTexture(VmaAllocator allocator, Texture& texture);

};

