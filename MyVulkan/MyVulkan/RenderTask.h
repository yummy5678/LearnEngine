#pragma once
#include "RenderScene.h"
#include "RenderConfig.h"
#include "SceneCamera.h"

// �`�施�߂ɕK�v�ȗv�f���܂Ƃ߂��\����
struct RenderTask
{
	RenderConfig config;
	RenderScene scene;
	SceneCamera camera;
};