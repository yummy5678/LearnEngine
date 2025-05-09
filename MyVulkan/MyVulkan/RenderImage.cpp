#include "RenderImage.h"

RenderImage::RenderImage() :
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
	m_ImageExtent(0),
	m_ColorImage(
		vk::ImageUsageFlagBits::eColorAttachment | 
		vk::ImageUsageFlagBits::eTransferDst |
		vk::ImageUsageFlagBits::eTransferSrc,
		vk::ImageAspectFlagBits::eColor,
		(vk::Format)VulkanDefine.UseColorFormat,
		vk::MemoryPropertyFlagBits::eDeviceLocal
	),
	m_DepthImage(
		vk::ImageUsageFlagBits::eDepthStencilAttachment,
		vk::ImageAspectFlagBits::eDepth,
		(vk::Format)VulkanDefine.UseDepthFormat,
		vk::MemoryPropertyFlagBits::eDeviceLocal
	),
	//m_ImageAspectFlag(),
	m_DrawCommand(),
	m_Fence(VK_NULL_HANDLE),
	m_ImageSet(),
	m_RenderFunctions()
{
}

RenderImage::~RenderImage()
{
	Cleanup();
}

vk::Device RenderImage::GetLogicalDevice()
{
	return m_LogicalDevice;
}

vk::Extent2D RenderImage::GetExtent()
{
	return { m_ColorImage.GetExtent().width, m_ColorImage.GetExtent().height };
}

RenderingImageSet RenderImage::GetImageSet()
{
	return m_ImageSet;
}

vk::Format RenderImage::GetColorFormat()
{
	return m_ColorImage.GetFormat();
}

vk::Format RenderImage::GetDepthFormat()
{
	return m_DepthImage.GetFormat();
}

void RenderImage::Initialize(VmaAllocator* allocator, vk::Extent2D extent)
{
	// VMA�ɕR�Â����Ă���I�u�W�F�N�g�̏����擾
	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);

	m_LogicalDevice = allocatorInfo.device;
	m_PhysicalDevice = allocatorInfo.physicalDevice;

	m_ImageExtent = extent;


	m_ColorImage.Initialize(allocator, extent);
	m_DepthImage.Initialize(allocator, extent);

	m_ImageSet = { m_ColorImage.GetImageSet(), m_DepthImage.GetImageSet() };
	
	m_DrawCommand.Create(allocatorInfo.device, allocatorInfo.physicalDevice);

	// �t�F���X���摜�̐������쐬
	vk::FenceCreateInfo fenceInfo;
	fenceInfo.pNext;
	fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
	m_Fence = m_LogicalDevice.createFence(fenceInfo);

}

void RenderImage::AddDrawTask(std::shared_ptr<RenderFunction> function)
{
	// �`�悷��֐���ێ����Ă���
	m_RenderFunctions.push_back(function);
}

void RenderImage::ExecuteDrawTask()
{
	// �t�F���X�̃V�O�i���҂�
	m_LogicalDevice.waitForFences(
		{ m_Fence },						// ���p����t�F���X�B
		VK_TRUE,							// �t�F���X���S�ăV�O�i����ԂɂȂ�܂ő҂�
		MAX_WAIT_TIME);						// �ő�ҋ@����
	m_LogicalDevice.resetFences(m_Fence);	// �t�F���X���V�O�i����Ԃɂ���

	

	// �`��R�}���h�̋L�^�J�n
	m_DrawCommand.BeginRendering(
		&m_ImageSet,
		{ {0, 0}, m_ImageExtent });

	// �I�u�W�F�N�g���p�C�v���C����ʂ��ĕ`��
	for (auto& function : m_RenderFunctions)
	{
		(*function)(m_DrawCommand.GetBuffer(), &(*function));
	}
	m_RenderFunctions.clear();

	// �R�}���h�̋L�^�̏I���ƃL���[�ւ̑��M
	m_DrawCommand.EndRendering(m_Fence, VK_NULL_HANDLE, VK_NULL_HANDLE, vk::ImageLayout::eTransferSrcOptimal);


}

void RenderImage::WriteImage(std::string fileName)
{
	GraphicsUtility::WriteImage(&m_ColorImage, fileName, m_Fence);
}

void RenderImage::Cleanup()
{
	// NULL�`�F�b�N
	if (m_LogicalDevice == VK_NULL_HANDLE) return; 
	if (m_PhysicalDevice == VK_NULL_HANDLE) return;

	printf("RendererBase��������܂�\n");

	// �R�}���h�̉��
	m_DrawCommand.Destroy();

	// �C���[�W�o�b�t�@�̉��
	m_DepthImage.Cleanup();
	m_ColorImage.Cleanup();

	// �����I�u�W�F�N�g�̉��
	if (m_Fence != VK_NULL_HANDLE)
	{
		m_LogicalDevice.destroyFence(m_Fence);
		m_Fence = VK_NULL_HANDLE;
	}


	// �����o�ϐ��̏�����
	m_RenderFunctions.clear();
	m_ImageSet = {};
	m_ImageExtent = 0;
	m_PhysicalDevice = VK_NULL_HANDLE;
	m_LogicalDevice = VK_NULL_HANDLE;
}

vk::ImageCreateInfo RenderImage::GetImageCreateInfo(vk::Extent2D extent, vk::Format format, vk::ImageUsageFlags usage)
{
	vk::ImageCreateInfo imageinfo;
	imageinfo.imageType = vk::ImageType::e2D;          // �摜�̎�ށi1D�`3D�j
	imageinfo.extent = vk::Extent3D{ extent, 1 };        // �摜�̕�
	imageinfo.mipLevels = 1;                           // �~�b�v�}�b�v���x���̐�
	imageinfo.arrayLayers = 1;                         // �摜�z��̃��x����
	imageinfo.format = format;     // �摜�̃t�H�[�}�b�g�^�C�v
	imageinfo.tiling = vk::ImageTiling::eLinear;       // �摜�f�[�^�̃^�C���z�u���@�i�œK�ȓǂݎ��̂��߂̔z�u�j
	imageinfo.initialLayout = vk::ImageLayout::eUndefined; // �쐬���̉摜�f�[�^�̃��C�A�E�g
	imageinfo.usage = usage; // �摜�̗p�r���`����r�b�g�t���O
	imageinfo.samples = vk::SampleCountFlagBits::e1;         // �}���`�T���v�����O�p�̃T���v����
	imageinfo.sharingMode = vk::SharingMode::eExclusive;		// �L���[�Ԃł̉摜���L���\���ǂ���

	return imageinfo;
}
