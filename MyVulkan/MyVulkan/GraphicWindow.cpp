#include "GraphicWindow.h"




GraphicWindow::GraphicWindow(VulkanInitializer& initializer) :
	m_pWindow(nullptr),
	//m_pInitializer(&initializer),
	m_AllocatorInfo(),
	m_Surface(initializer),
	m_Swapchain(initializer),
	m_Fences(),
	m_DrawCommands(),
	m_CurrentImageSet(),
	m_SwapcheinFrameCount(0),
	m_CurrentIndex(0),
	m_RenderFunctions()
{
	//if (m_pInitializer == nullptr)
	//throw std::runtime_error("コンストラクタには中身のあるポインタを指定してください！");

	m_AllocatorInfo.instance = VK_NULL_HANDLE;
	m_AllocatorInfo.physicalDevice = VK_NULL_HANDLE;
	m_AllocatorInfo.device = VK_NULL_HANDLE;
}

GraphicWindow::~GraphicWindow()
{
}

void GraphicWindow::init(VulkanInitializer* initializer, const std::string wName, const int width, const int height)
{

	//if (m_pInitializer != initializer) 
	//	throw std::runtime_error("コンストラクタに入れたイニシャライザと同じものを指定してください！");

	//m_pInitializer = initializer;
	auto pAllocator = initializer->GetPVmaAllocator();
	vmaGetAllocatorInfo(*pAllocator, &m_AllocatorInfo);

	// Set GLFW to NOT work with OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


	m_pWindow = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);

	m_Surface.CreateWindowSurface(m_AllocatorInfo.instance, m_pWindow);

	if (initializer->CheckSupportSurface(m_Surface.GetSurface()) == false)
	{
		// スワップチェイン出来ないエラーメッセージ
		throw std::runtime_error("サーフェスがスワップチェインに対応していません！");
		return;
	}

	m_Swapchain.Create(pAllocator, m_Surface.GetSurface());

	m_SwapcheinFrameCount = m_Swapchain.GetFrameCount();

	// スワップチェインで使用する画像の枚数
	auto renderingImage = m_Swapchain.GetRenderingImageSet();
	m_DrawCommands.resize(m_SwapcheinFrameCount);
	for (uint32_t i = 0; i < m_SwapcheinFrameCount; i++)
	{
		m_DrawCommands[i].Create(
			m_AllocatorInfo.device,
			m_AllocatorInfo.physicalDevice);
	}

	// フェンスを画像の数だけ作成
	vk::FenceCreateInfo fenceInfo;
	fenceInfo.pNext;
	fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
	m_Fences.resize(m_SwapcheinFrameCount);

	vk::Device logicalDevice = m_AllocatorInfo.device;
	for (uint32_t i = 0; i < m_SwapcheinFrameCount; i++)
	{
		m_Fences[i] = logicalDevice.createFence(fenceInfo);
	}
}

void GraphicWindow::Cleanup()
{
	if (m_AllocatorInfo.device == nullptr) return;
	vk::Device logicalDevice = m_AllocatorInfo.device;

	// 描画コマンドの解放
	for (auto& command: m_DrawCommands)
	{
		command.Destroy();
	}

	// フェンスの解放
	for (auto& fence : m_Fences)
	{
		logicalDevice.destroyFence(fence);
	}

	// スワップチェインの解放
	m_Swapchain.Cleanup();

	//サーフェスの解放
	m_Surface.Cleanup();

	// ウィンドウの後始末
	if (m_pWindow != nullptr)
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}

	// その他各値の初期化
	m_RenderFunctions.clear();
	m_CurrentIndex = 0;
	m_SwapcheinFrameCount = 0;
	//m_pInitializer = nullptr;
	m_pWindow = nullptr;
	m_AllocatorInfo.instance = VK_NULL_HANDLE;
	m_AllocatorInfo.physicalDevice = VK_NULL_HANDLE;
	m_AllocatorInfo.device = VK_NULL_HANDLE;

}

void GraphicWindow::AddDrawTask(std::shared_ptr<RenderFunction> function)
{
	// 後で描画コマンドを発行する関数自体を入れるようにしたい
	m_RenderFunctions.push_back(function);
}

void GraphicWindow::ExecuteDrawTask()
{
	vk::Device logicalDevice = m_AllocatorInfo.device;

	//vk::Fence fence = m_Fences[m_CurrentIndex];
	vk::Fence fence = m_Fences[0];

	// 次のインデックス
	logicalDevice.waitForFences(
	    //{ m_Fences[m_CurrentIndex] },	// 利用するフェンス達
	    { fence },	// 利用するフェンス達
	    VK_TRUE,						// フェンスが全てシグナル状態になるまで待つ
	    UINT64_MAX);					// 最大待機時間
	logicalDevice.resetFences(fence);	// フェンスを非シグナル状態にする

	auto imageSet = m_Swapchain.GetRenderingImageSet();

	// 描画コマンドの記録開始
	m_DrawCommands[m_CurrentIndex].BeginRendering(
		&imageSet,
		m_Swapchain.GetImageAvailableSemaphore(),
		{{0, 0}, m_Swapchain.GetExtent()});

	// オブジェクトをパイプラインを通して描画
	for (auto& function : m_RenderFunctions)
	{
		(*function)(m_DrawCommands[m_CurrentIndex].GetBuffer(), &(*function));
	}
	m_RenderFunctions.clear();
	
	// コマンドの記録の終了とキューへの送信
	m_DrawCommands[m_CurrentIndex].EndRendering(fence, vk::ImageLayout::ePresentSrcKHR);

	// 描画した画像をウィンドウに表示
	Presentation();

	// カウントを進める
	m_CurrentIndex = (m_CurrentIndex + 1) % m_SwapcheinFrameCount;
}

void GraphicWindow::Presentation()
{
	m_Swapchain.UpdateFrame();
}

RenderingImageSet GraphicWindow::GetImageSet()
{
	return m_Swapchain.GetRenderingImageSet();
}

vk::Format GraphicWindow::GetColorFormat()
{
	return m_Swapchain.GetColorFormat();
}

vk::Format GraphicWindow::GetDepthFormat()
{
	return m_Swapchain.GetDepthFormat();
}

vk::Extent2D GraphicWindow::GetExtent()
{
	return m_Swapchain.GetExtent();
}

GLFWwindow* GraphicWindow::GetPointer()
{
	return m_pWindow;
}

int GraphicWindow::checkCloseWindow()
{
	return glfwWindowShouldClose(m_pWindow);
}
