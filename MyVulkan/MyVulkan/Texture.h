#pragma once
#include <cstdint>
#include <vector>

enum TextureChannel
{
	TEXTURE_CHANNEL_UNKNOWN		= 0,
	TEXTURE_CHANNEL_GREY		= 1,
	TEXTURE_CHANNEL_GREY_ALPHA	= 2,
	TEXTURE_CHANNEL_RGB			= 3,
	TEXTURE_CHANNEL_RGB_ALPHA	= 4,
	TEXTURE_CHANNEL_MAX_NUM
};

using ColorBit = uint8_t;	// 1色はあたりのデータサイズは8ビット(0～255)

struct Texture
{
	uint32_t width = 0;		// 画像の幅
	uint32_t height = 0;	// 画像の高さ
	TextureChannel channel = TEXTURE_CHANNEL_UNKNOWN;	// 1画素に含まれるチャンネル数	
	//uint8_t* data = nullptr;		// RAM上の画像データのポインタ
	std::vector<ColorBit> pixelData;		// RAM上の画像データのポインタ
};

