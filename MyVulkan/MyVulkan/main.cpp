#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define TINYGLTF_USE_CPP14
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#define TINYGLTF_NOEXCEPTION
#define TINYGLTF_IMPLEMENTATION
#define JSON_NOEXCEPTION

//#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#pragma comment(linker, "/ENTRY:mainCRTStartup")
#include <stdexcept>
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
//#include "spdlog/spdlog.h"
#include "GraphicsDefine.h"
#include "VulkanInitializer.h"
#include "GltfLoader.h"
#include "GraphicWindow.h"
#include "RenderConfig.h"
#include "SwapchainRenderer.h"

#include "HelloTriangleRenderer.h"
#include "RenderImage.h"



int main()
{
	VulkanInitializer	vulkanInitializer;
	GraphicWindow		mainWindow(vulkanInitializer);		//�����_���[
	RenderConfig		renderConfig(vulkanInitializer);	// �`����@�̌`�������߂�I�u�W�F�N�g
	HelloTriangleRenderer triangleRenderer(vulkanInitializer);
	RenderImage			renderTarget;


	// ���������_���[�̏���������肭�����Ȃ�������A�v�����I��
	if (vulkanInitializer.init() == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	vk::Device logicalDevice = vulkanInitializer.GetLogicalDevice();
	vk::PhysicalDevice physicalDevice = vulkanInitializer.GetPhysicalDevice();
	VmaAllocator* pAllocator = vulkanInitializer.GetPVmaAllocator();


	// �E�B���h�E���쐬
	mainWindow.init(&vulkanInitializer, "Vulkan Window", windowWidth, windowHeight);
	renderTarget.Initialize(pAllocator, { windowWidth, windowHeight });

	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	RenderObject m_Object;		//�\�����郂�f�����X�g

	// ���f�������[�h
	//std::vector<MeshObject> mesh = FileLoader::LoadGLTF("models/AliciaSolid.vrm");
	std::vector<MeshObject> mesh = FileLoader::LoadGLTF("models/Cube/Cube.gltf");

	// �`��p�̃N���X�Ƀ��f�����Z�b�g
	m_Object.SetMesh(vulkanInitializer.GetPVmaAllocator(), &mesh);
	std::vector<RenderObject*> objContainer = { &m_Object };

	// �J�����N���X���쐬
	SceneCamera camera(pAllocator);
	camera.UpdateBuffer(ViewProjection(1.0));

	
	renderConfig.Initialize(logicalDevice, physicalDevice, &mainWindow);

	
	triangleRenderer.Initialize(&renderTarget);

	renderTarget.AddDrawTask(triangleRenderer.GetRenderFunction());
	renderTarget.ExecuteDrawTask();
	renderTarget.WriteImage("writeImage.bmp");

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

		mainWindow.AddDrawTask(renderConfig.GetRenderFunction(&objContainer, &camera));
		//mainWindow.AddDrawTask(triangleRenderer.GetRenderFunction());
		mainWindow.ExecuteDrawTask();

	}

	renderTarget.AddDrawTask(renderConfig.GetRenderFunction(&objContainer, &camera));
	renderTarget.ExecuteDrawTask();
	renderTarget.WriteImage("endImage.bmp");

	//ToDo �O���t�B�N�X�p�C�v���C���̉���������쐬����
	// �쐬�����I�u�W�F�N�g�͍쐬�����̂Ƌt�̏��Ԃŉ������
	renderTarget.Cleanup();
	triangleRenderer.Cleanup();
	renderConfig.Cleanup();
	mainWindow.Cleanup();
	vulkanInitializer.Cleanup();

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