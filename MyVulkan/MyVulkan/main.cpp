#pragma once

#include <stdexcept>
#include <vector>
#include <iostream>
#include "GraphicsDefine.h"
#include "GameWindow.h"
#include "Renderer.h"	//Vulkanのレンダラー処理をまとめている

GameWindow window;
VulkanRenderer vulkanRenderer;	//レンダラー

int main()
{
	// ウィンドウを作成
	window.init("Vulkan Window", windowWidth, windowHeight);

	// もしレンダラーの初期化が上手くいかなかったらアプリを終了
	if (vulkanRenderer.init(window) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	//無限ループ(ウィンドウの終了フラグが立つまで)
	while (!window.checkCloseWindow())
	{
		//ここで毎フレーム更新を行う
		glfwPollEvents();
	}

	vulkanRenderer.cleanup();

	// 作成したウィンドウを片づける
	window.kill();

	return EXIT_SUCCESS;
}

/*/////////////////////////////////////////////
*バルカン(Vulkan)とは？
*//////////////////////////////////////////////
/*
//OpenGLの後継で作られた。
//OpenGLよりも動作を細かくコントロールできるようになっている
//ただし、その分コードが長くなりがち
//
//
*/



/*/////////////////////////////////////////////
*レンダラー(Renderer)とは？
*//////////////////////////////////////////////



/*/////////////////////////////////////////////
*画面表示までに必要なもの・流れ
*//////////////////////////////////////////////


/*/////////////////////////////////////////////
* vulkan.hppとvulkan.hの違い
*//////////////////////////////////////////////