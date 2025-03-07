#pragma once
#include "Texture.h"

class ImageGenerator
{
public:
	ImageGenerator()  = default;
	~ImageGenerator() = default;

	void GenerateColorGradetion(Texture& drawData);


private:
	Texture imageData;




};

