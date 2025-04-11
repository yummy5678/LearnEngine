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
	//throw std::runtime_error("�R���X�g���N�^�ɂ͒��g�̂���|�C���^���w�肵�Ă��������I");

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
	//	throw std::runtime_error("�R���X�g���N�^�ɓ��ꂽ�C�j�V�����C�U�Ɠ������̂��w�肵�Ă��������I");

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
		// �X���b�v�`�F�C���o���Ȃ��G���[���b�Z�[�W
		throw std::runtime_error("�T�[�t�F�X���X���b�v�`�F�C���ɑΉ����Ă��܂���I");
		return;
	}

	m_Swapchain.Create(pAllocator, m_Surface.GetSurface());

	m_SwapcheinFrameCount = m_Swapchain.GetFrameCount();

	// �X���b�v�`�F�C���Ŏg�p����摜�̖���
	auto renderingImage = m_Swapchain.GetRenderingImageSet();
	m_DrawCommands.resize(m_SwapcheinFrameCount);
	for (uint32_t i = 0; i < m_SwapcheinFrameCount; i++)
	{
		m_DrawCommands[i].Create(
			m_AllocatorInfo.device,
			m_AllocatorInfo.physicalDevice);
	}

	// �t�F���X���摜�̐������쐬
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

	// �`��R�}���h�̉��
	for (auto& command: m_DrawCommands)
	{
		command.Destroy();
	}

	// �t�F���X�̉��
	for (auto& fence : m_Fences)
	{
		logicalDevice.destroyFence(fence);
	}

	// �X���b�v�`�F�C���̉��
	m_Swapchain.Cleanup();

	//�T�[�t�F�X�̉��
	m_Surface.Cleanup();

	// �E�B���h�E�̌�n��
	if (m_pWindow != nullptr)
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}

	// ���̑��e�l�̏�����
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
	// ��ŕ`��R�}���h�𔭍s����֐����̂�����悤�ɂ�����
	m_RenderFunctions.push_back(function);
}

void GraphicWindow::ExecuteDrawTask()
{
	vk::Device logicalDevice = m_AllocatorInfo.device;

	//vk::Fence fence = m_Fences[m_CurrentIndex];
	vk::Fence fence = m_Fences[0];

	// ���̃C���f�b�N�X
	logicalDevice.waitForFences(
	    //{ m_Fences[m_CurrentIndex] },	// ���p����t�F���X�B
	    { fence },	// ���p����t�F���X�B
	    VK_TRUE,						// �t�F���X���S�ăV�O�i����ԂɂȂ�܂ő҂�
	    UINT64_MAX);					// �ő�ҋ@����
	logicalDevice.resetFences(fence);	// �t�F���X���V�O�i����Ԃɂ���

	auto imageSet = m_Swapchain.GetRenderingImageSet();

	// �`��R�}���h�̋L�^�J�n
	m_DrawCommands[m_CurrentIndex].BeginRendering(
		&imageSet,
		m_Swapchain.GetImageAvailableSemaphore(),
		{{0, 0}, m_Swapchain.GetExtent()});

	// �I�u�W�F�N�g���p�C�v���C����ʂ��ĕ`��
	for (auto& function : m_RenderFunctions)
	{
		(*function)(m_DrawCommands[m_CurrentIndex].GetBuffer(), &(*function));
	}
	m_RenderFunctions.clear();
	
	// �R�}���h�̋L�^�̏I���ƃL���[�ւ̑��M
	m_DrawCommands[m_CurrentIndex].EndRendering(fence, vk::ImageLayout::ePresentSrcKHR);

	// �`�悵���摜���E�B���h�E�ɕ\��
	Presentation();

	// �J�E���g��i�߂�
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
