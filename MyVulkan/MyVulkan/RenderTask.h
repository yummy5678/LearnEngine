#pragma once
#include "RenderConfig.h"
#include "SceneCamera.h"

// �`�施�߂ɕK�v�ȗv�f���܂Ƃ߂��\����
struct RenderTask
{
	RenderConfig config;
	std::vector<RenderObject> objects;
	SceneCamera camera;
};