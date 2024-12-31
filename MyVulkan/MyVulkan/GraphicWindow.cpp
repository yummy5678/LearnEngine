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
	// Set GLFW to NOT work with OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	vk::Instance instance = m_pInitializer->GetInstance();

	m_pWindow = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);

	m_Surface.CreateWindowSurface(instance, m_pWindow);

	if (m_pInitializer->CheckSupportSurface(m_Surface.GetSurface()) == false)
	{
		// スワップチェイン出来ないエラーメッセージ
		return;
	}

	m_GraphicController.Create(m_pInitializer->GetPVmaAllocator(), m_Surface.GetSurface());
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

RendererBase* GraphicWindow::GetRenderer()
{
	return &m_GraphicController;
}



int GraphicWindow::checkCloseWindow()
{
	return glfwWindowShouldClose(m_pWindow);
}
