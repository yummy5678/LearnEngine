#include "ImageGenerator.h"

void ImageGenerator::GenerateColorGradetion(Texture& drawData)
{
	drawData.width = 255;
	drawData.height = 255;
	drawData.channel = TEXTURE_CHANNEL_RGB;	// RGB�̎O�F�f�[�^(�����x����)

	size_t dataSize = sizeof(ColorBit) * drawData.channel * drawData.width * drawData.height;

	// ToDo :�J���[�O���f�[�V�����̉摜���R���s���[�g�V�F�[�_�[��ʂ��č쐬���Ă݂���
}
