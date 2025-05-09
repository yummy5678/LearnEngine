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
	//throw std::runtime_error("�R���X�g���N�^�ɂ͒��g�̂���|�C���^���w�肵�Ă��������I");

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

	// �X���b�v�`�F�C���Ŏg�p����t���[���̖���
	m_SwapcheinFrameCount = m_Swapchain.GetFrameCount();

	// �X���b�v�`�F�C���Ŏg�p����摜�̖���
	//auto renderingImage = m_Swapchain.GetRenderingImageSet();

	m_DrawCommands.resize(m_SwapcheinFrameCount); // DrawCommand�̓R�s�[�֎~�Ȃ̂ŗ̈悾���m��
	for (uint32_t i = 0; i < m_SwapcheinFrameCount; i++)
	{
		//m_DrawCommands.emplace_back(); // ���[�u���R�s�[���s�v
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

	// �Z�}�t�H���摜�̐������쐬
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
	// NULL�`�F�b�N
	if (m_AllocatorInfo.device == nullptr) return;
	vk::Device logicalDevice = m_AllocatorInfo.device;

	// �`��R�}���h�̉��
	for (auto& command: m_DrawCommands)
	{
		command.Destroy();
	}
	m_DrawCommands.clear();

	// �t�F���X�̉��
	for (auto& fence : m_Fences)
	{
		logicalDevice.destroyFence(fence);
	}
	m_Fences.clear();

	// �Z�}�t�H�̉��
	for (auto& semaphore : m_ImageAvailableSemaphores)
	{
		logicalDevice.destroySemaphore(semaphore);
	}
	m_ImageAvailableSemaphores.clear();

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

	// 1. �X���b�v�`�F�[���摜�̎擾�i�`��Ώۂ̃C���f�b�N�X���擾�j
	

	// 2. �O�t���[���̕`�抮����ҋ@
	vk::Fence currentFence = m_Fences[m_CurrentIndex];


	// ���̃C���f�b�N�X
	logicalDevice.waitForFences(
	    { currentFence },	// ���p����t�F���X
	    VK_TRUE,						// �t�F���X���S�ăV�O�i����ԂɂȂ�܂ő҂�
		MAX_WAIT_TIME);					// �ő�ҋ@����
	logicalDevice.resetFences(currentFence);	// �t�F���X���V�O�i����Ԃɂ���

	// AcquireNextImage���g������
	m_Swapchain.UpdateSwapchainNextFrame(m_ImageAvailableSemaphores[m_CurrentIndex]);
	const uint32_t frameIndex = m_Swapchain.GetUseImageIndex();	// �X�V�����C���f�b�N�X�ԍ����擾

	// 3. �`��R�}���h�̋L�^�J�n
	RenderingImageSet imageSet = m_Swapchain.GetRenderingImageSet();
	DrawCommand& useCommand = m_DrawCommands[frameIndex];

	// �`��R�}���h�̋L�^�J�n
	useCommand.BeginRendering(
		&imageSet,
		{{0, 0}, m_Swapchain.GetExtent()});

	// 4. �o�^���ꂽ�`�揈�������s
	// �I�u�W�F�N�g���p�C�v���C����ʂ��ĕ`��
	for (auto& function : m_RenderFunctions)
	{
		(*function)(useCommand.GetBuffer(), &(*function));

		// �����Ń��j�t�H�[���o�b�t�@���X�V
	}
	m_RenderFunctions.clear();
	
	// 5. �`��R�}���h�̏I���Ƒ��M�i�t�F���X��ݒ�j
	// �R�}���h�̋L�^�̏I���ƃL���[�ւ̑��M
	std::vector<vk::Semaphore> waitSemaphores(1);
	waitSemaphores[0] = m_ImageAvailableSemaphores[m_CurrentIndex];
	std::vector<vk::Semaphore> signalSemaphores(1); 
	signalSemaphores[0] = m_DrawCommands[m_CurrentIndex].GetRenderFinishedSemaphore();
	useCommand.EndRendering(
		currentFence,
		&waitSemaphores,
		&signalSemaphores,
		vk::ImageLayout::ePresentSrcKHR);

	// 6. �E�B���h�E�Ƀv���[���e�[�V�����i������ vkQueuePresentKHR ���ĂԐ݌v�Ɂj
	// �`�悵���摜���E�B���h�E�ɕ\��
	//Presentation();
	m_Swapchain.PresentFrame(&signalSemaphores);

	// 7. �C���f�b�N�X�̍X�V�iAcquireNextImageKHR�Ŏ���擾�����̂ŏȗ��j
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
