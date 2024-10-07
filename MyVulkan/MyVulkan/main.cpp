#pragma once

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stdexcept>
#include <vector>
#include <iostream>
#include "GraphicsDefine.h"
#include "GameWindow.h"
#include "Renderer.h"		//Vulkanのレンダラー処理をまとめている

GameWindow m_pWindow;
VulkanRenderer vulkanRenderer;	//レンダラー

int main()
{

	
	RenderConfig renderConfig;	// 描画方法の形式を決めるオブジェクト
	renderConfig.Initialize();

	// ウィンドウを作成
	m_pWindow.init("Vulkan Window", windowWidth, windowHeight);

	// もしレンダラーの初期化が上手くいかなかったらアプリを終了
	if (vulkanRenderer.init(m_pWindow) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	Scene scene;
	scene.Initialize();

	//int helicopter = vulkanRenderer.createMeshModel("Models/uh60.obj");
	vulkanRenderer.setRenderConfig(renderConfig);

	//無限ループ(ウィンドウの終了フラグが立つまで)
	while (!m_pWindow.checkCloseWindow())
	{
		//ここで毎フレーム更新を行う
		glfwPollEvents();
		scene.Update();


		vulkanRenderer.draw();
	}

	vulkanRenderer.cleanup();

	// 作成したウィンドウを片づける
	m_pWindow.kill();

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