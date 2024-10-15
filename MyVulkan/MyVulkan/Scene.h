#pragma once
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/ext/matrix_transform.hpp>
#include "SceneObject.h"
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
	void Render(RenderConfig &pipeline);
	// void Render(RenderPipeline &pipeline, Camera camera);

	std::vector<SceneObject> GetObject();


private:
	SceneObject m_Object; //表示するモデルリスト(座標とか拡大率はまだ持ってない)
	//std::vector<Texture>	m_Texture;

	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;





};

