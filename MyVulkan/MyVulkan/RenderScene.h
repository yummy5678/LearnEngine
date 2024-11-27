#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>///////////////////
#include <stb/stb_image.h>
#include <glm/ext/matrix_transform.hpp>
#include "RenderObject.h"
#include "SceneCamera.h"
#include "MeshManager.h"


// ���b�V�����f���⃂�f���̍��W�Ȃǂ�
// �����Ă����N���X�B
// ���̃N���X�ƃJ�����N���X���p�C�v���C���ɓn���Ďg��

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
	RenderObject		m_Object;		//�\�����郂�f�����X�g
	SceneCamera		m_Camera;		// �X���b�v�`�F�C���ō����ɕ`�悷��p�̃J�����ݒ�


	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;





};

