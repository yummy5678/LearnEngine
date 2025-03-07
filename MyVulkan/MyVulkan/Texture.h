#pragma once
#include <cstdint>

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
	TextureChannel channel = TEXTURE_CHANNEL_UNKNOWN;	// 1画素に含まれるチャンネル数	
	uint32_t width = 0;		// 画像の幅
	uint32_t height = 0;	// 画像の高さ
	uint8_t* data = 0;		// RAM上の画像データのポインタ
};

