#pragma once
#include "RenderConfig.h"
#include "RenderScene.h"

// �����_���[�ɓn�������܂Ƃ߂��\����
// �l�̂ǂ��炩��nullptr�������Ă���΂��̏���
// ��������悤�ɐ݌v���Ă���B
struct RenderingUnit
{
	RenderConfig*	config = nullptr;
	RenderScene*	scene  = nullptr;
};

