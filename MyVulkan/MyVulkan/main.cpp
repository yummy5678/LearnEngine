#pragma once

#include <stdexcept>
#include <vector>
#include <iostream>
#include "GraphicsDefine.h"
#include "GameWindow.h"
#include "Renderer.h"	//Vulkan�̃����_���[�������܂Ƃ߂Ă���

GameWindow window;
VulkanRenderer vulkanRenderer;	//�����_���[

int main()
{
	// �E�B���h�E���쐬
	window.init("Vulkan Window", windowWidth, windowHeight);

	// ���������_���[�̏���������肭�����Ȃ�������A�v�����I��
	if (vulkanRenderer.init(window) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	//�������[�v(�E�B���h�E�̏I���t���O�����܂�)
	while (!window.checkCloseWindow())
	{
		//�����Ŗ��t���[���X�V���s��
		glfwPollEvents();
	}

	vulkanRenderer.cleanup();

	// �쐬�����E�B���h�E��ЂÂ���
	window.kill();

	return EXIT_SUCCESS;
}

/*/////////////////////////////////////////////
*�o���J��(Vulkan)�Ƃ́H
*//////////////////////////////////////////////
/*
//OpenGL�̌�p�ō��ꂽ�B
//OpenGL����������ׂ����R���g���[���ł���悤�ɂȂ��Ă���
//�������A���̕��R�[�h�������Ȃ肪��
//
//
*/



/*/////////////////////////////////////////////
*�����_���[(Renderer)�Ƃ́H
*//////////////////////////////////////////////



/*/////////////////////////////////////////////
*��ʕ\���܂łɕK�v�Ȃ��́E����
*//////////////////////////////////////////////


/*/////////////////////////////////////////////
* vulkan.hpp��vulkan.h�̈Ⴂ
*//////////////////////////////////////////////