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

	window = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
}

void GameWindow::kill()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow* GameWindow::getWindowPointer()
{
	return window;
}

int GameWindow::checkCloseWindow()
{
	return glfwWindowShouldClose(window);
}
