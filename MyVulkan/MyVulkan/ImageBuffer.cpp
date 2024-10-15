#include "ImageBuffer.h"

VImageBuffer::VImageBuffer()
{
}

VImageBuffer::~VImageBuffer()
{
}

vk::Image VImageBuffer::GetImage()
{
	return m_Buffer;
}
