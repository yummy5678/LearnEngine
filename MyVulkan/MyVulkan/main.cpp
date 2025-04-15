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
	GraphicWindow		mainWindow(vulkanInitializer);		//レンダラー
	RenderConfig		renderConfig(vulkanInitializer);	// 描画方法の形式を決めるオブジェクト
	HelloTriangleRenderer triangleRenderer(vulkanInitializer);
	RenderImage			renderTarget;


	// もしレンダラーの初期化が上手くいかなかったらアプリを終了
	if (vulkanInitializer.init() == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	vk::Device logicalDevice = vulkanInitializer.GetLogicalDevice();
	vk::PhysicalDevice physicalDevice = vulkanInitializer.GetPhysicalDevice();
	VmaAllocator* pAllocator = vulkanInitializer.GetPVmaAllocator();


	// ウィンドウを作成
	mainWindow.init(&vulkanInitializer, "Vulkan Window", windowWidth, windowHeight);
	renderTarget.Initialize(pAllocator, { windowWidth, windowHeight });

	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	RenderObject m_Object;		//表示するモデルリスト

	// モデルをロード
	//std::vector<MeshObject> mesh = FileLoader::LoadGLTF("models/AliciaSolid.vrm");
	std::vector<MeshObject> mesh = FileLoader::LoadGLTF("models/Cube/Cube.gltf");

	// 描画用のクラスにモデルをセット
	m_Object.SetMesh(vulkanInitializer.GetPVmaAllocator(), &mesh);
	std::vector<RenderObject*> objContainer = { &m_Object };

	// カメラクラスを作成
	SceneCamera camera(pAllocator);
	camera.UpdateBuffer(ViewProjection(1.0));

	
	renderConfig.Initialize(logicalDevice, physicalDevice, &mainWindow);

	
	triangleRenderer.Initialize(&renderTarget);

	renderTarget.AddDrawTask(triangleRenderer.GetRenderFunction());
	renderTarget.ExecuteDrawTask();
	renderTarget.WriteImage("writeImage.bmp");

	//無限ループ(ウィンドウの終了フラグが立つまで)
	while (!mainWindow.checkCloseWindow())
	{
		//ここで毎フレーム更新を行う
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

	//ToDo グラフィクスパイプラインの解放処理を作成する
	// 作成したオブジェクトは作成したのと逆の順番で解放する
	renderTarget.Cleanup();
	triangleRenderer.Cleanup();
	renderConfig.Cleanup();
	mainWindow.Cleanup();
	vulkanInitializer.Cleanup();

	return EXIT_SUCCESS;
}

/*/////////////////////////////////////////////
*バルカン(Vulkan)とは？
*//////////////////////////////////////////////
/*
//OpenGLの後継で作られた。
//OpenGLよりも動作を細かくコントロールできるようになっている
//ただし、その分コードが長くなりがち
//Vulkanのほうが処理が最適化されて早くなるみたいなことを聞くが、私はよく知らない。
//どっちも大差ない気がする
*/

/*/////////////////////////////////////////////
* GameWindowとは？
*//////////////////////////////////////////////
//三角形を表示するウィンドウを作成するクラス。GLSLライブラリの力を借りている
//Rendererにはサーフェス(ウィンドウの表示領域の情報)を読み取るために渡している


/*/////////////////////////////////////////////
*レンダラー(Renderer)とは？
*//////////////////////////////////////////////
//VulkanRendererはVulkanのインスタンス作成から破棄までを担ってもらうクラス
//詳細はRenderer.hのファイルに書いてある




/*/////////////////////////////////////////////
*画面表示までに必要なもの・流れ
*//////////////////////////////////////////////


/*/////////////////////////////////////////////
* vulkan.hppとvulkan.hの違い
*//////////////////////////////////////////////