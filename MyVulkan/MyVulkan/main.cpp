#pragma once
#define STB_IMAGE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stdexcept>
#include <vector>
#include <iostream>
#include "GraphicsDefine.h"
#include "VulkanInitializer.h"
#include "GraphicWindow.h"
#include "RenderScene.h"
#include "RenderConfig.h"
#include "SwapchainRenderer.h"


int main()
{		
	VulkanInitializer	vulkanInitializer;	//�����_���[
	GraphicWindow		mainWindow(&vulkanInitializer);

	// ���������_���[�̏���������肭�����Ȃ�������A�v�����I��
	if (vulkanInitializer.init() == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	// �E�B���h�E���쐬
	mainWindow.init("Vulkan Window", windowWidth, windowHeight);

	RenderConfig renderConfig;	// �`����@�̌`�������߂�I�u�W�F�N�g
	renderConfig.Initialize(
		vulkanInitializer.GetLogicalDevice(), 
		mainWindow.GetWindowSize(),
		mainWindow.GetColorFormat(),
		mainWindow.GetDepthFormat());

	RenderScene scene;
	scene.Initialize(vulkanInitializer.GetPVmaAllocator());

	//�������[�v(�E�B���h�E�̏I���t���O�����܂�)
	while (!mainWindow.checkCloseWindow())
	{
		//�����Ŗ��t���[���X�V���s��
		glfwPollEvents();
		scene.Update();

		mainWindow.UpdateRenderer({ { &renderConfig, &scene } });
	}

	vulkanInitializer.cleanup();

	// �쐬�����E�B���h�E��ЂÂ���
	mainWindow.kill();

	return EXIT_SUCCESS;
}

/*/////////////////////////////////////////////
*�o���J��(Vulkan)�Ƃ́H
*//////////////////////////////////////////////
/*
//OpenGL�̌�p�ō��ꂽ�B
//OpenGL����������ׂ����R���g���[���ł���悤�ɂȂ��Ă���
//�������A���̕��R�[�h�������Ȃ肪��
//Vulkan�̂ق����������œK������đ����Ȃ�݂����Ȃ��Ƃ𕷂����A���͂悭�m��Ȃ��B
//�ǂ������卷�Ȃ��C������
*/

/*/////////////////////////////////////////////
* GameWindow�Ƃ́H
*//////////////////////////////////////////////
//�O�p�`��\������E�B���h�E���쐬����N���X�BGLSL���C�u�����̗͂��؂�Ă���
//Renderer�ɂ̓T�[�t�F�X(�E�B���h�E�̕\���̈�̏��)��ǂݎ�邽�߂ɓn���Ă���


/*/////////////////////////////////////////////
*�����_���[(Renderer)�Ƃ́H
*//////////////////////////////////////////////
//VulkanRenderer��Vulkan�̃C���X�^���X�쐬����j���܂ł�S���Ă��炤�N���X
//�ڍׂ�Renderer.h�̃t�@�C���ɏ����Ă���




/*/////////////////////////////////////////////
*��ʕ\���܂łɕK�v�Ȃ��́E����
*//////////////////////////////////////////////


/*/////////////////////////////////////////////
* vulkan.hpp��vulkan.h�̈Ⴂ
*//////////////////////////////////////////////