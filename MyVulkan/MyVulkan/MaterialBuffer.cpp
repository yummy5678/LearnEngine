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

void VMaterial::SetTexture(VmaAllocator allocator, Texture& texture)
{
	m_Texture.SetImage(allocator, texture, );
}
