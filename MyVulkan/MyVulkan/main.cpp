#pragma once

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stdexcept>
#include <vector>
#include <iostream>
#include "GraphicsDefine.h"
#include "GameWindow.h"
#include "Renderer.h"		//Vulkan�̃����_���[�������܂Ƃ߂Ă���

GameWindow m_pWindow;
VulkanRenderer vulkanRenderer;	//�����_���[

int main()
{

	
	RenderConfig renderConfig;	// �`����@�̌`�������߂�I�u�W�F�N�g
	renderConfig.Initialize();

	// �E�B���h�E���쐬
	m_pWindow.init("Vulkan Window", windowWidth, windowHeight);

	// ���������_���[�̏���������肭�����Ȃ�������A�v�����I��
	if (vulkanRenderer.init(m_pWindow) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	Scene scene;
	scene.Initialize();

	//int helicopter = vulkanRenderer.createMeshModel("Models/uh60.obj");
	vulkanRenderer.setRenderConfig(renderConfig);

	//�������[�v(�E�B���h�E�̏I���t���O�����܂�)
	while (!m_pWindow.checkCloseWindow())
	{
		//�����Ŗ��t���[���X�V���s��
		glfwPollEvents();
		scene.Update();


		vulkanRenderer.draw();
	}

	vulkanRenderer.cleanup();

	// �쐬�����E�B���h�E��ЂÂ���
	m_pWindow.kill();

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