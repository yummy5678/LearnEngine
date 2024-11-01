#pragma once
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/ext/matrix_transform.hpp>
#include "SceneObject.h"
#include "SceneCamera.h"
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
	// void Render(RenderPipeline &pipeline, Camera camera);

	std::vector<SceneObject>	GetObjects();
	//std::vector<SceneCamera>	GetCameras();
	SceneCamera					GetMainCamera();


private:
	SceneObject m_Object;		//�\�����郂�f�����X�g
	SceneCamera m_MainCamera;	// �X���b�v�`�F�C���ō����ɕ`�悷��p�̃J�����ݒ�

	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;





};

