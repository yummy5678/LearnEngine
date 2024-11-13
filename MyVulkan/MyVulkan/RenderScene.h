#pragma once
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/ext/matrix_transform.hpp>
#include "SceneObject.h"
#include "SceneCamera.h"
#include "MeshManager.h"
#include "RenderConfig.h"




// メッシュモデルやモデルの座標などを
// 持っておくクラス。
// このクラスとカメラクラスをパイプラインに渡して使う



class RenderScene
{
public:
	RenderScene();
	~RenderScene();

	void Initialize(VmaAllocator allocator, RenderConfig* config);
	void Update();
	// void Render(RenderPipeline &pipeline, Camera camera);

	std::vector<SceneObject>	GetObjects();
	//std::vector<SceneCamera>	GetCameras();
	SceneCamera					GetMainCamera();
	RenderConfig*				GetRenderConfig();

private:
	VmaAllocator	m_Allocator;
	RenderConfig*	m_pConfig;
	SceneObject		m_Object;		//表示するモデルリスト
	SceneCamera		m_Camera;	// スワップチェインで高速に描画する用のカメラ設定


	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;





};

