#include "GraphicWindow.h"



GraphicWindow::GraphicWindow(VulkanInitializer& initializer) :
	m_pWindow(nullptr),
	m_pInitializer(&initializer),
	m_Surface(initializer),
	m_Swapchain(initializer),
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
		// �X���b�v�`�F�C���o���Ȃ��G���[���b�Z�[�W
		throw std::runtime_error("�T�[�t�F�X���X���b�v�`�F�C���ɑΉ����Ă��܂���I");
		return;
	}

	m_Swapchain.Create(m_pInitializer->GetPVmaAllocator(), m_Surface.GetSurface());

	m_DrawCommand.Create(
		*m_pInitializer->GetPLogicalDevice(), 
		*m_pInitializer->GetPPhysicalDevice(),
		&m_Swapchain);
}

void GraphicWindow::kill()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void GraphicWindow::AddDrawTask(std::shared_ptr<RenderFunction> function)
{
	// ��ŕ`��R�}���h�𔭍s����֐����̂�����悤�ɂ�����
	m_RenderFunctions.push_back(function);
}

void GraphicWindow::ExecuteDrawTask()
{
	vk::Device* logicalDevice = m_pInitializer->GetPLogicalDevice();

	//std::vector<vk::Fence> usingFences = { m_DrawCommand.GetFence() };

	//logicalDevice->waitForFences(
	//	usingFences,							// ���p����t�F���X�B
	//	VK_TRUE,								// �t�F���X���S�ăV�O�i����ԂɂȂ�܂ő҂�
	//	UINT64_MAX);							// �ő�ҋ@����

	//logicalDevice->resetFences(usingFences);	// �t�F���X���V�O�i����Ԃɂ���

	// �`��R�}���h�̋L�^�J�n
	m_DrawCommand.BeginRendering({ {0, 0}, m_Swapchain.GetExtent() });

	// �I�u�W�F�N�g���p�C�v���C����ʂ��ĕ`��
	for (auto& function : m_RenderFunctions)
	{
		//task.config->BindRenderingCommand(m_DrawCommand.GetBuffer(), task.objects, task.camera);
		(*function)(m_DrawCommand.GetBuffer(), &(*function));
	}


	// �R�}���h�̋L�^�̏I���ƃL���[�ւ̑��M
	m_DrawCommand.EndRendering(vk::ImageLayout::ePresentSrcKHR);

	// �`�悵���摜���E�B���h�E�ɕ\��
	Presentation();
}

void GraphicWindow::Presentation()
{
	m_Swapchain.UpdateFrame(m_DrawCommand.GetImageAvableSemaphore());
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
