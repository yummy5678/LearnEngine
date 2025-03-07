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

using ColorBit = uint8_t;	// 1�F�͂�����̃f�[�^�T�C�Y��8�r�b�g(0�`255)

struct Texture
{
	TextureChannel channel = TEXTURE_CHANNEL_UNKNOWN;	// 1��f�Ɋ܂܂��`�����l����	
	uint32_t width = 0;		// �摜�̕�
	uint32_t height = 0;	// �摜�̍���
	uint8_t* data = 0;		// RAM��̉摜�f�[�^�̃|�C���^
};

