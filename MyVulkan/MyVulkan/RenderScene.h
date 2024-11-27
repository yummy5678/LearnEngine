#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>///////////////////
#include <stb/stb_image.h>
#include <glm/ext/matrix_transform.hpp>
#include "RenderObject.h"
#include "SceneCamera.h"
#include "MeshManager.h"


// メッシュモデルやモデルの座標などを
// 持っておくクラス。
// このクラスとカメラクラスをパイプラインに渡して使う

class RenderScene
{
public:
	RenderScene();
	~RenderScene();

	void Initialize(VmaAllocator* allocator);
	void Update();
	// void Render(RenderPipeline &pipeline, Camera camera);

	std::vector<RenderObject>	GetObjects();
	//std::vector<SceneCamera>	GetCameras();
	SceneCamera					GetMainCamera();


private:
	VmaAllocator*	m_Allocator;
	RenderObject		m_Object;		//表示するモデルリスト
	SceneCamera		m_Camera;		// スワップチェインで高速に描画する用のカメラ設定


	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;





};

