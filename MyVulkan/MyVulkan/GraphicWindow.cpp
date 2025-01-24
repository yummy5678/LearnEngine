#include "GraphicWindow.h"



GraphicWindow::GraphicWindow(VulkanInitializer& initializer) :
	m_pWindow(nullptr),
	m_pInitializer(&initializer),
	m_Surface(initializer),
	m_GraphicController(initializer),
	m_DrawCommand(),
	m_DrawTasks()
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

void GraphicWindow::AddDrawTask(RenderConfig* pRenderConfig, std::vector<RenderObject>* pObjects, SceneCamera* pCamera)
{
	m_DrawTasks.push_back(RenderTask{ pRenderConfig, pObjects, pCamera });
}

void GraphicWindow::ExecuteDrawTask()
{
	m_DrawCommand.BeginRendering({ {0, 0}, m_GraphicController.GetFrameExtent() });

	for (const auto& task : m_DrawTasks)
	{
		auto pipeline = task.config->GetPipeline();
		auto pipelineLayout = task.config->GetPipelineLayout();
		auto descriptorSets = task.config->GetPShaderConfiguer()->GetDescriptorSets();

		for (auto& model : *task.objects)
		{
		    for (auto& mesh : *model.GetMeshes())
		    {
		        // m_TextureDescriptor.Update(mesh.GetPMaterial()->GetTextureImageView(), mesh.GetPMaterial()->GetSampler());
		        m_DrawCommand.RenderMesh(pipeline, pipelineLayout, &descriptorSets, &mesh, model.GetPTransform());
		    }
		}
	}

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
