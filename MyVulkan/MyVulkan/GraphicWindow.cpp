#include "GraphicWindow.h"



GraphicWindow::GraphicWindow(VulkanInitializer& initializer) :
	m_pWindow(nullptr),
	m_pInitializer(&initializer),
	m_Surface(initializer),
	m_Swapchain(initializer),
	m_DrawCommands(),
	m_RenderingImage(),
	m_CurrentIndex(0),
	m_NextIndex(0),
	m_RenderFunctions()
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

	m_Swapchain.Create(m_pInitializer->GetPVmaAllocator(), m_Surface.GetSurface());

	// スワップチェインで使用する画像の枚数
	m_RenderingImage = m_Swapchain.GetImageSets();
	m_DrawCommands.resize(m_RenderingImage.size());
	for (uint32_t i = 0; i < m_RenderingImage.size(); i++)
	{
		m_DrawCommands[i].Create(
			*m_pInitializer->GetPLogicalDevice(),
			*m_pInitializer->GetPPhysicalDevice());
	}
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
	vk::Device* logicalDevice = m_pInitializer->GetPLogicalDevice();

	m_NextIndex = m_Swapchain.GetUseImageIndex();

	// 描画コマンドの記録開始
	m_DrawCommands[m_CurrentIndex].BeginRendering(
		&m_RenderingImage[m_NextIndex],
		m_Swapchain.GetImageAvailableSemaphore(),
		{{0, 0}, m_Swapchain.GetExtent()});

	// オブジェクトをパイプラインを通して描画
	for (auto& function : m_RenderFunctions)
	{
		//task.config->BindRenderingCommand(m_DrawCommand.GetBuffer(), task.objects, task.camera);
		(*function)(m_DrawCommands[m_CurrentIndex].GetBuffer(), &(*function));
	}


	// コマンドの記録の終了とキューへの送信
	m_DrawCommands[m_CurrentIndex].EndRendering(vk::ImageLayout::ePresentSrcKHR);
	printf("%d :スワップチェーンの描画が完了\n", m_NextIndex);
	printf("%d\n", m_Swapchain.GetImageAvailableSemaphore());


	// 描画した画像をウィンドウに表示
	Presentation();

	// カウントを進める
	m_CurrentIndex = (m_CurrentIndex + 1) % m_RenderingImage.size();
}

void GraphicWindow::Presentation()
{
	m_Swapchain.UpdateFrame();
}

GLFWwindow* GraphicWindow::GetPointer()
{
	return m_pWindow;
}

RenderTarget* GraphicWindow::GetRenderer()
{
	return &m_Swapchain;
}



int GraphicWindow::checkCloseWindow()
{
	return glfwWindowShouldClose(m_pWindow);
}
