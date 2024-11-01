#pragma once
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/ext/matrix_transform.hpp>
#include "SceneObject.h"
#include "SceneCamera.h"
#include "MeshManager.h"
#include "RenderPipeline.h"



// メッシュモデルやモデルの座標などを
// 持っておくクラス。
// このクラスとカメラクラスをパイプラインに渡して使う



class Scene
{
public:
	Scene();
	~Scene();

	void Initialize();
	void Update();
	// void Render(RenderPipeline &pipeline, Camera camera);

	std::vector<SceneObject>	GetObjects();
	//std::vector<SceneCamera>	GetCameras();
	SceneCamera					GetMainCamera();


private:
	SceneObject m_Object;		//表示するモデルリスト
	SceneCamera m_MainCamera;	// スワップチェインで高速に描画する用のカメラ設定

	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;





};

