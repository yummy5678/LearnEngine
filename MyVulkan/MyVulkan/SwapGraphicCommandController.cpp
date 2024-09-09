#include "SwapGraphicCommandController.h"

SwapGraphicCommandController::SwapGraphicCommandController(CDeviceExtensionManager& deviceExtensionManager):
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
	m_Surface(VK_NULL_HANDLE),
	m_SwapchainGenerator(deviceExtensionManager),
	m_RenderpassGenerator(),
	m_FramebufferGenerator(),
	m_PipelineGenerator(),
	m_SynchroGenerator()
{
}

SwapGraphicCommandController::~SwapGraphicCommandController()
{
}

void SwapGraphicCommandController::Initialize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	m_LogicalDevice = logicalDevice;
	m_PhysicalDevice = physicalDevice;
	m_Surface = surface;

	//�X���b�v�`�F�[���̍쐬
	m_SwapchainGenerator.Create(logicalDevice, physicalDevice, surface);
	auto swapchainInfo = m_SwapchainGenerator.GetSwapchainInfo();
	auto extent = swapchainInfo.imageExtent;
	auto swapchainImage = m_SwapchainGenerator.GetImages();


	//�R�}���h�o�b�t�@�̍쐬
	m_CommandGenerator.Create(logicalDevice, physicalDevice, swapchainImage.GetSize());

	//�����_�[�p�X�̍쐬
	m_RenderpassGenerator.Create(logicalDevice, swapchainImage.GetFomat());
	auto renderPass = m_RenderpassGenerator.GetRenderpass();

	//�p�C�v���C���̍쐬
	m_PipelineGenerator.Create(logicalDevice, extent, renderPass);
	auto graphicsPipeline = m_PipelineGenerator.GetPipeline();

	//�t���[���o�b�t�@�̍쐬
	m_FramebufferGenerator.Create(logicalDevice, swapchainImage.GetImageViews(), renderPass, extent);
	auto framebuffers = m_FramebufferGenerator.GetFramebuffers();

	//�R�}���h�̋L�^
	m_CommandGenerator.Create(logicalDevice, physicalDevice, swapchainInfo.minImageCount);
	auto commandBuffers = m_CommandGenerator.GetCommandBuffers();
	//m_CommandGenerator.RecordGraphicCommands(framebuffers, renderPass, windowExtent, graphicsPipeline);

	m_CommandGenerator.DrawFrame(commandBuffers[0], renderPass, framebuffers[0], { {0,0},extent }, graphicsPipeline);
	//WriteVulkanImage("../frame0.bmp", swapchainImage.GetImageData()[0], windowExtent);


	m_SynchroGenerator.Create(logicalDevice);


}

void SwapGraphicCommandController::DrawFrame()
{

}

void SwapGraphicCommandController::PresentFrame()
{

	// -- �`�悵���C���[�W����ʂɃv���[���g�i�\���j --
	// �\���̂��߂̃v���[���g�����Z�b�g
	vk::PresentInfoKHR presentInfo;
	presentInfo.pNext;
	presentInfo.pResults;
	presentInfo.setImageIndices(imageIndex);	// �X���b�v�`�F�[�����̃C���[�W�C���f�b�N�X
	
	presentInfo.pSwapchains = &swapchain;		// �v���[���g����X���b�v�`�F�[��
	presentInfo.pWaitSemaphores = &renderFinished[currentFrame];// �����_�����O�����Z�}�t�H��҂�
						
	
	// �v���[���g�L���[���擾���āA�C���[�W��\��
	auto presentQueue = logicalDevice.getQueue(queueSelector.GetPresentationIndex(m_SurfaceGenerator.GetSurface()), 0);
	result = presentQueue.presentKHR(presentInfo);
	
	// �v���[���g�Ɏ��s�����ꍇ�A�G���[���b�Z�[�W�𓊂���
	if (result != vk::Result::eSuccess)
	{
		throw std::runtime_error("�C���[�W�̕\���Ɏ��s���܂����I");
	}
	
	// �t���[���C���f�b�N�X�����ɐi�߂܂��i�t���[�����𒴂��Ȃ��悤�Ƀ��[�v�j
	currentFrame = (currentFrame + 1) % MAX_FRAME_DRAWS;
}
