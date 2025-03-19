#include "RenderImage.h"

RenderImage::RenderImage() :
	m_LogicalDevice(),
	m_PhysicalDevice(),
	m_ColorImage(vk::ImageAspectFlagBits::eColor),
	m_DepthImage(vk::ImageAspectFlagBits::eDepth),
	m_ImageExtent(),
	m_ImageFormat(vk::Format::eR8G8B8A8Unorm),
	m_ImageAspectFlag(),
	m_ColorImageUsage(vk::ImageUsageFlagBits::eColorAttachment),
	m_DepthImageUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment),
	m_DrawCommand(),
	m_Fence(),
	m_ImageSet(),
	m_RenderFunctions()
{
}

RenderImage::~RenderImage()
{
}

vk::Extent2D RenderImage::GetExtent()
{
	return vk::Extent2D();
}

RenderingImageSet RenderImage::GetImageSet()
{
	return m_ImageSet;
}

vk::Format RenderImage::GetColorFormat()
{
	return vk::Format();
}

vk::Format RenderImage::GetDepthFormat()
{
	return vk::Format();
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

	//m_ImageSet = { m_ColorImage.GetImageSet(), m_DepthImage.GetImageSet() };
	
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
	// ���̃C���f�b�N�X
	m_LogicalDevice.waitForFences(
		{ m_Fence },						// ���p����t�F���X�B
		VK_TRUE,							// �t�F���X���S�ăV�O�i����ԂɂȂ�܂ő҂�
		UINT64_MAX);						// �ő�ҋ@����
	m_LogicalDevice.resetFences(m_Fence);	// �t�F���X���V�O�i����Ԃɂ���

	

	// �`��R�}���h�̋L�^�J�n
	m_DrawCommand.BeginRendering(
		&m_ImageSet,
		VK_NULL_HANDLE,							// �t�F���X
		{ {0, 0}, m_ImageExtent });

	// �I�u�W�F�N�g���p�C�v���C����ʂ��ĕ`��
	for (auto& function : m_RenderFunctions)
	{
		(*function)(m_DrawCommand.GetBuffer(), &(*function));
	}
	m_RenderFunctions.clear();

	// �R�}���h�̋L�^�̏I���ƃL���[�ւ̑��M
	m_DrawCommand.EndRendering(m_Fence, vk::ImageLayout::eGeneral);


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
