#pragma once
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/ext/matrix_transform.hpp>
#include "SceneObject.h"
#include "MeshManager.h"
#include "RenderPipeline.h"


// ���b�V�����f���⃂�f���̍��W�Ȃǂ�
// �����Ă����N���X�B
// ���̃N���X�ƃJ�����N���X���p�C�v���C���ɓn���Ďg��



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
	SceneObject m_Object; //�\�����郂�f�����X�g(���W�Ƃ��g�嗦�͂܂������ĂȂ�)
	//std::vector<Texture>	m_Texture;

	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;





};

