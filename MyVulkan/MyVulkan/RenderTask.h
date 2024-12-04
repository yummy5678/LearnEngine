#pragma once
#include "RenderConfig.h"
#include "SceneCamera.h"

// 描画命令に必要な要素をまとめた構造体
struct RenderTask
{
	RenderConfig config;
	std::vector<RenderObject> objects;
	SceneCamera camera;
};