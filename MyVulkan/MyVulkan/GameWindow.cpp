#include "GameWindow.h"


GameWindow::GameWindow()
{
}

GameWindow::~GameWindow()
{
}

void GameWindow::init(std::string wName, const int width, const int height)
{
	// Initialise GLFW
	glfwInit();

	// Set GLFW to NOT work with OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_pWindow = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
}

void GameWindow::kill()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

GLFWwindow* GameWindow::GetPointer()
{
	return m_pWindow;
}

int GameWindow::checkCloseWindow()
{
	return glfwWindowShouldClose(m_pWindow);
}
