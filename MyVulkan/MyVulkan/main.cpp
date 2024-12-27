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
	VulkanInitializer	vulkanInitializer;	//レンダラー
	GraphicWindow		mainWindow(vulkanInitializer);

	// もしレンダラーの初期化が上手くいかなかったらアプリを終了
	if (vulkanInitializer.init() == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	// ウィンドウを作成
	mainWindow.init("Vulkan Window", windowWidth, windowHeight);

	RenderConfig renderConfig(vulkanInitializer);	// 描画方法の形式を決めるオブジェクト
	renderConfig.Initialize(
		vulkanInitializer.GetLogicalDevice(), 
		mainWindow.GetWindowSize(),
		mainWindow.GetColorFormat(),
		mainWindow.GetDepthFormat());


	auto allocator = vulkanInitializer.GetPVmaAllocator();

	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	RenderObject	m_Object;		//表示するモデルリスト

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

		mainWindow.AddRenderQueue({ renderConfig, {m_Object}, SceneCamera() });
	}

	vulkanInitializer.cleanup();

	// 作成したウィンドウを片づける
	mainWindow.kill();

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