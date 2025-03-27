#pragma once
#include "stb/stb_image.h"
#include "stb/stb_image_write.h" 
#include "StagingImageBuffer.h"
#include "VImageBufferBase.h"


static void WriteImage(VImageBufferBase* vImageBuffer, std::string writeFileName)
{
	VStagingImageBuffer stagingBuffer;

	auto allocator = vImageBuffer->GetUsingAllocator();
	auto imageSize = vImageBuffer->GetExtent();
	vk::Image image = vImageBuffer->GetImageBuffer();

	stagingBuffer.Initialize(allocator, imageSize.width, imageSize.height, 4);

	Texture imageData;
	stagingBuffer.TransferImageBufferToHostData(image, &imageData);

	stbi_write_bmp(writeFileName.c_str(), 
		imageData.width, imageData.height, 
		imageData.channel, 
		imageData.pixelData.data())

}




