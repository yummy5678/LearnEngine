#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>	//�E�B���h�E�̍쐬��GLFW���C�u�������g��
#include <string>

//�E�B���h�E�쐬�p�̃N���X
class GameWindow
{
public:
	//�R���X�g���N�^�ƃf�X�g���N�^
	GameWindow();
	~GameWindow();

	//�E�B���h�E�̏������ƍ쐬
	void init(std::string wName = "Standard Window", const int width = 800, const int height = 600);
	//�E�B���h�E�̏I������
	void kill();

	//�E�B���h�E�̃|�C���^�[��n��
	GLFWwindow* getWindowPointer();

	//�E�B���h�E�̏I���t���O�������Ă��邩�m�F
	int checkCloseWindow();

private:
	//GLFW�E�B���h�E�̃|�C���^�[
	GLFWwindow* window = nullptr;
};

