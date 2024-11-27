#include "GraphicWindow.h"



GraphicWindow::GraphicWindow(VulkanInitializer& initializer) :
	m_pWindow(nullptr),
	m_pInitializer(&initializer),
	m_Surface(initializer),
	m_GraphicController(initializer)
{
}

GraphicWindow::~GraphicWindow()
{
}

void GraphicWindow::init(const std::string wName, const int width, const int height)
{
	// Initialise GLFW
	glfwInit();

	// Set GLFW to NOT work with OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	vk::Instance instance = m_pInitializer->GetInstance();
	vk::PhysicalDevice physicalDevice = m_pInitializer->GetPhysicalDevice();
	vk::Device logicalDevice = m_pInitializer->GetLogicalDevice();

	m_pWindow = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);

	m_Surface.CreateWindowSurface(instance, m_pWindow);

	if (m_pInitializer->CheckSupportSurface(m_Surface.GetSurface()) == false)
	{
		// スワップチェイン出来ないエラーメッセージ
	}

	m_GraphicController.Create(logicalDevice, physicalDevice, m_Surface.GetSurface());
}

void GraphicWindow::kill()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void GraphicWindow::AddRenderQueue(RenderTask renderTask)
{
	m_RenderTasks.push_back(renderTask);
}

void GraphicWindow::ExecuteRenderQueue()
{
	m_GraphicController.UpdateFrame(m_RenderTasks);
}

GLFWwindow* GraphicWindow::GetPointer()
{
	return m_pWindow;
}

vk::Extent2D GraphicWindow::GetWindowSize()
{
	return m_Surface.GetCapabilities(m_pInitializer->GetPhysicalDevice()).currentExtent;
}

vk::Format GraphicWindow::GetColorFormat()
{
	return m_GraphicController.GetImages().GetColorFormat();
}

vk::Format GraphicWindow::GetDepthFormat()
{
	return m_GraphicController.GetImages().GetDepthFormat();
}

int GraphicWindow::checkCloseWindow()
{
	return glfwWindowShouldClose(m_pWindow);
}
