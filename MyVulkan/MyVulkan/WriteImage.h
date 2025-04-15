#pragma once
//#include "stb/stb_image.h"
//#include "stb/stb_image_write.h"
#include "GltfLoader.h"
#include "StagingImageBuffer.h"
#include "VImageBufferBase.h"

namespace GraphicsUtility
{
	static void WriteImage(VImageBufferBase* vImageBuffer, std::string writeFileName, vk::Fence fence)
	{
		VStagingImageBuffer stagingBuffer;

		auto allocator = vImageBuffer->GetUsingAllocator();
		auto imageSize = vImageBuffer->GetExtent();

		VmaAllocatorInfo allocatorInfo;
		vmaGetAllocatorInfo(*allocator, &allocatorInfo);
		vk::Device logicalDevice = allocatorInfo.device;

		stagingBuffer.Initialize(allocator, imageSize.width, imageSize.height, TEXTURE_CHANNEL_RGB_ALPHA);

		Texture imageData;
		stagingBuffer.TransferImageBufferToHostData(vImageBuffer, &imageData, fence);

		//std::fill(imageData.pixelData.begin(), imageData.pixelData.end(), (uint8_t)255);


		stbi_write_bmp(writeFileName.c_str(), 
			imageData.width, imageData.height, 
			imageData.channel, 
			imageData.pixelData.data());

	}

}





