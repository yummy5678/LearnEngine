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
		// �X���b�v�`�F�C���o���Ȃ��G���[���b�Z�[�W
		throw std::runtime_error("�T�[�t�F�X���X���b�v�`�F�C���ɑΉ����Ă��܂���I");
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
	// ��ŕ`��R�}���h�𔭍s����֐����̂�����悤�ɂ�����
	m_RenderFunctions.push_back(function);
}

void GraphicWindow::ExecuteDrawTask()
{
	// �`��R�}���h�̋L�^�J�n
	m_DrawCommand.BeginRendering(0, { {0, 0}, m_GraphicController.GetExtent() });

	// �I�u�W�F�N�g���p�C�v���C����ʂ��ĕ`��
	for (auto& function : m_RenderFunctions)
	{
		//task.config->BindRenderingCommand(m_DrawCommand.GetBuffer(), task.objects, task.camera);
		(*function)(m_DrawCommand.GetBuffer(),  this);
	}


	// �R�}���h�̋L�^�̏I���ƃL���[�ւ̑��M
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
