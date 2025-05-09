#include "GraphicWindow.h"




GraphicWindow::GraphicWindow(VulkanInitializer& initializer) :
	m_pWindow(nullptr),
	//m_pInitializer(&initializer),
	m_AllocatorInfo(),
	m_Surface(initializer),
	m_Swapchain(initializer),
	m_ImageAvailableSemaphores(),
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
	Cleanup();
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

	// スワップチェインで使用するフレームの枚数
	m_SwapcheinFrameCount = m_Swapchain.GetFrameCount();

	// スワップチェインで使用する画像の枚数
	//auto renderingImage = m_Swapchain.GetRenderingImageSet();

	m_DrawCommands.resize(m_SwapcheinFrameCount); // DrawCommandはコピー禁止なので領域だけ確保
	for (uint32_t i = 0; i < m_SwapcheinFrameCount; i++)
	{
		//m_DrawCommands.emplace_back(); // ムーブもコピーも不要
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

	// セマフォを画像の数だけ作成
	vk::SemaphoreCreateInfo semaphoreInfo;
	semaphoreInfo.pNext;
	semaphoreInfo.flags;
	m_ImageAvailableSemaphores.resize(m_SwapcheinFrameCount);
	for (uint32_t i = 0; i < m_SwapcheinFrameCount; i++)
	{
		m_ImageAvailableSemaphores[i] = logicalDevice.createSemaphore(semaphoreInfo);
	}
}

void GraphicWindow::Cleanup()
{
	// NULLチェック
	if (m_AllocatorInfo.device == nullptr) return;
	vk::Device logicalDevice = m_AllocatorInfo.device;

	// 描画コマンドの解放
	for (auto& command: m_DrawCommands)
	{
		command.Destroy();
	}
	m_DrawCommands.clear();

	// フェンスの解放
	for (auto& fence : m_Fences)
	{
		logicalDevice.destroyFence(fence);
	}
	m_Fences.clear();

	// セマフォの解放
	for (auto& semaphore : m_ImageAvailableSemaphores)
	{
		logicalDevice.destroySemaphore(semaphore);
	}
	m_ImageAvailableSemaphores.clear();

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

	// 1. スワップチェーン画像の取得（描画対象のインデックスを取得）
	

	// 2. 前フレームの描画完了を待機
	vk::Fence currentFence = m_Fences[m_CurrentIndex];


	// 次のインデックス
	logicalDevice.waitForFences(
	    { currentFence },	// 利用するフェンス
	    VK_TRUE,						// フェンスが全てシグナル状態になるまで待つ
		MAX_WAIT_TIME);					// 最大待機時間
	logicalDevice.resetFences(currentFence);	// フェンスを非シグナル状態にする

	// AcquireNextImageを使いたい
	m_Swapchain.UpdateSwapchainNextFrame(m_ImageAvailableSemaphores[m_CurrentIndex]);
	const uint32_t frameIndex = m_Swapchain.GetUseImageIndex();	// 更新したインデックス番号を取得

	// 3. 描画コマンドの記録開始
	RenderingImageSet imageSet = m_Swapchain.GetRenderingImageSet();
	DrawCommand& useCommand = m_DrawCommands[frameIndex];

	// 描画コマンドの記録開始
	useCommand.BeginRendering(
		&imageSet,
		{{0, 0}, m_Swapchain.GetExtent()});

	// 4. 登録された描画処理を実行
	// オブジェクトをパイプラインを通して描画
	for (auto& function : m_RenderFunctions)
	{
		(*function)(useCommand.GetBuffer(), &(*function));

		// ここでユニフォームバッファを更新
	}
	m_RenderFunctions.clear();
	
	// 5. 描画コマンドの終了と送信（フェンスを設定）
	// コマンドの記録の終了とキューへの送信
	std::vector<vk::Semaphore> waitSemaphores(1);
	waitSemaphores[0] = m_ImageAvailableSemaphores[m_CurrentIndex];
	std::vector<vk::Semaphore> signalSemaphores(1); 
	signalSemaphores[0] = m_DrawCommands[m_CurrentIndex].GetRenderFinishedSemaphore();
	useCommand.EndRendering(
		currentFence,
		&waitSemaphores,
		&signalSemaphores,
		vk::ImageLayout::ePresentSrcKHR);

	// 6. ウィンドウにプレゼンテーション（内部で vkQueuePresentKHR を呼ぶ設計に）
	// 描画した画像をウィンドウに表示
	//Presentation();
	m_Swapchain.PresentFrame(&signalSemaphores);

	// 7. インデックスの更新（AcquireNextImageKHRで次回取得されるので省略可）
	m_CurrentIndex = (m_CurrentIndex + 1) % m_SwapcheinFrameCount;
}

vk::Device GraphicWindow::GetLogicalDevice()
{
	return m_AllocatorInfo.device;
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
