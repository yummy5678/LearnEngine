#pragma once
#include "RenderConfig.h"
#include "RenderScene.h"

// レンダラーに渡す情報をまとめた構造体
// 値のどちらかにnullptrが入っていればその情報を
// 無視するように設計している。
struct RenderingUnit
{
	RenderConfig*	config = nullptr;
	RenderScene*	scene  = nullptr;
};

