#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE


#include <stdexcept>
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "GraphicsDefine.h"
#include "VulkanInitializer.h"
#include "GraphicWindow.h"
#include "RenderConfig.h"
#include "SwapchainRenderer.h"
#include "GltfLoader.h"


int main()
{
	VulkanInitializer	vulkanInitializer;
	GraphicWindow		mainWindow(vulkanInitializer);	//�����_���[

	// ���������_���[�̏���������肭�����Ȃ�������A�v�����I��
	if (vulkanInitializer.init() == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	// �E�B���h�E���쐬
	mainWindow.init("Vulkan Window", windowWidth, windowHeight);

	auto allocator = vulkanInitializer.GetPVmaAllocator();

	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	RenderObject m_Object;		//�\�����郂�f�����X�g

	// ���f�������[�h
	MeshObject mesh;
	LoadGLTF("models/AliciaSolid.vrm", mesh);

	// ���[�h�������f����VRAM�Ɉړ�
	VMeshObject vMesh;
	vMesh.SetMeshObject(vulkanInitializer.GetPVmaAllocator(), mesh);

	// �`��p�̃N���X�Ƀ��f�����Z�b�g
	m_Object.SetMesh(vulkanInitializer.GetPVmaAllocator(), &vMesh);
	std::vector<RenderObject> objContainer = { m_Object };

	// �J�����N���X���쐬
	SceneCamera camera;

	RenderConfig renderConfig(vulkanInitializer);	// �`����@�̌`�������߂�I�u�W�F�N�g
	renderConfig.Initialize(
		mainWindow.GetRenderer(),
		&objContainer,
		&camera);

	//�������[�v(�E�B���h�E�̏I���t���O�����܂�)
	while (!mainWindow.checkCloseWindow())
	{
		//�����Ŗ��t���[���X�V���s��
		glfwPollEvents();

		float now = (float)glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		angle += 10.0f * deltaTime;
		if (angle > 360.0f) { angle -= 360.0f; }

		glm::mat4 testMat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		testMat = glm::rotate(testMat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		m_Object.SetTransform(testMat);

		mainWindow.AddRenderQueue({ &renderConfig, &objContainer, &SceneCamera() });
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