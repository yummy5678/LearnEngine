#include "GraphicWindow.h"



GraphicWindow::GraphicWindow(VulkanInitializer& initializer) :
	m_pWindow(nullptr),
	m_pInitializer(&initializer),
	m_Surface(initializer),
	m_GraphicController(initializer),
	m_DrawCommand(),
	m_RenderFunctions()
	/*m_DrawTasks()*/
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
		throw std::runtime_error("サーフェスがスワップチェインに対応していません！");
		return;
	}

	m_GraphicController.Create(m_pInitializer->GetPVmaAllocator(), m_Surface.GetSurface());

	m_DrawCommand.Create(
		m_pInitializer->GetPLogicalDevice(), 
		m_pInitializer->GetPPhysicalDevice(),
		m_GraphicController.GetImageSets());
}

void GraphicWindow::kill()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void GraphicWindow::AddDrawTask(std::shared_ptr<RenderFunction> function)
{
	// 後で描画コマンドを発行する関数自体を入れるようにしたい
	m_RenderFunctions.push_back(function);
}

void GraphicWindow::ExecuteDrawTask()
{
	// 描画コマンドの記録開始
	m_DrawCommand.BeginRendering(0, { {0, 0}, m_GraphicController.GetExtent() });

	// オブジェクトをパイプラインを通して描画
	for (auto& function : m_RenderFunctions)
	{
		//task.config->BindRenderingCommand(m_DrawCommand.GetBuffer(), task.objects, task.camera);
		(*function)(m_DrawCommand.GetBuffer(),  this);
	}


	// コマンドの記録の終了とキューへの送信
	m_DrawCommand.EndRendering();

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
