#include "ImageGenerator.h"

void ImageGenerator::GenerateColorGradetion(Texture& drawData)
{
	drawData.width = 255;
	drawData.height = 255;
	drawData.channel = TEXTURE_CHANNEL_RGB;	// RGBの三色データ(透明度無し)

	size_t dataSize = sizeof(ColorBit) * drawData.channel * drawData.width * drawData.height;

	// ToDo :カラーグラデーションの画像をコンピュートシェーダーを通して作成してみたい
}
