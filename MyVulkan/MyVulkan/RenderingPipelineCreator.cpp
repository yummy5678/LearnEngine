#include "RenderingPipelineCreator.h"


RenderingPipelineCreator::RenderingPipelineCreator(VulkanInitializer& initializer) :
	m_pLogicalDevice(nullptr),
	m_Pipeline(VK_NULL_HANDLE),
	m_PipelineInfo(),
	m_PipelineLayout(VK_NULL_HANDLE),
	m_PipelineLayoutInfo(),
	m_TextureDescriptor()
{
	m_ClassName = "PipelineGenerator";

	if (initializer.IsInitialized() == true) 
		printf("�C�j�V�����C�U�[���C�j�V�����C�Y����O�Ƀp�C�v���C���̃R���X�g���N�^��ǂ�ł�������");
	initializer.GetPDeviceExtension()->UseDynamicRendering();
	m_pLogicalDevice = initializer.GetPLogicalDevice();

}

RenderingPipelineCreator::~RenderingPipelineCreator()
{
}

void RenderingPipelineCreator::Create(
	vk::Device* pLogicalDevice,
	vk::Extent2D extent,
	vk::Rect2D scissor,
	vk::Format colorFormat,
	vk::Format depthFormat,
	vk::PipelineVertexInputStateCreateInfo* pVertexInputState,
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfos,
	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts,
	std::vector<vk::PushConstantRange> pushConstantRanges)
{
	m_bCreated = true;

	//m_TextureDescriptor.CreateSingleDescriptorSet();
	//�p�C�v���C�����C�A�E�g�̍쐬	//���͍��Ȃ��Ă�������
	//std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = { m_TextureDescriptor.GetDescriptorSetLayout() };
	CreatePipelineLayout(pLogicalDevice, descriptorSetLayouts, pushConstantRanges);

	//�p�C�v���C���̍쐬
	CreateGraphicsPipeline(m_pLogicalDevice, extent, scissor, colorFormat, depthFormat, shaderStageInfos, pVertexInputState);
}

void RenderingPipelineCreator::Destroy()
{
	if (m_pLogicalDevice == nullptr) return;

	m_pLogicalDevice->destroyPipelineLayout(m_PipelineLayout);
	m_pLogicalDevice->destroyPipeline(m_Pipeline);

	//�p�C�v���C���̍쐬��ɕs�v�ɂȂ����V�F�[�_�[���W���[����j��
	//vkDestroyShaderModule(logicalDevice, fragmentShaderModule, nullptr);
	//vkDestroyShaderModule(logicalDevice, vertexShaderModule, nullptr);
}

vk::Pipeline RenderingPipelineCreator::GetPipeline()
{
	CheckCreated();
	return m_Pipeline;
}

vk::PipelineLayout RenderingPipelineCreator::GetPipelineLayout()
{
	CheckCreated();
	return m_PipelineLayout;
}

void RenderingPipelineCreator::CreatePipelineLayout(vk::Device* pLogicalDevice, std::vector<vk::DescriptorSetLayout> descriptorSetLayouts, std::vector<vk::PushConstantRange> pushConstantRanges)
{
	vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
	pipelineLayoutCreateInfo.setSetLayouts(descriptorSetLayouts);
	pipelineLayoutCreateInfo.setPushConstantRanges(pushConstantRanges);

	m_PipelineLayout = pLogicalDevice->createPipelineLayout(pipelineLayoutCreateInfo); 
}

void RenderingPipelineCreator::CreateGraphicsPipeline(vk::Device* pLogicalDevice, vk::Extent2D extent, vk::Rect2D scissor, vk::Format colorFormat, vk::Format depthFormat, std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfos, vk::PipelineVertexInputStateCreateInfo* pVertexInputState)
{
	if (!m_PipelineLayout)	throw std::runtime_error("�O���t�B�N�X�p�C�v���C���̍쐬�O�Ƀp�C�v���C�����C�A�E�g���쐬���Ă�������!");

	// �r���[�|�[�g�̐ݒ�����쐬
#pragma region viewport
	vk::Viewport viewport;
	viewport.setX(0.0f);									// x���W�̊J�n�ʒu
	viewport.setY(0.0f);									// y���W�̊J�n�ʒu
	viewport.setWidth((float)extent.width);					// �r���[�|�[�g�̕�
	viewport.setHeight((float)extent.height);				// �r���[�|�[�g�̍���
	viewport.setMinDepth(0.0f);								// �t���[���o�b�t�@�̍ŏ��[�x
	viewport.setMaxDepth(1.0f);								// �t���[���o�b�t�@�̍ő�[�x

	// �r���[�|�[�g�ƃV�U�[�̏�Ԃ�ݒ肷�邽�߂̍\���̂��쐬
	vk::PipelineViewportStateCreateInfo viewportStateInfo;
	viewportStateInfo.setViewportCount(1);		// �r���[�|�[�g�̐�
	viewportStateInfo.setPViewports(&viewport);	// �r���[�|�[�g�̃|�C���^
	viewportStateInfo.setScissorCount(1);			// �V�U�[�̐�
	viewportStateInfo.setPScissors(&scissor);		// �V�U�[�̃|�C���^
#pragma endregion viewport

	// ���X�^���C�U�[�̐ݒ�
#pragma region rasterizationInfo
	/* //////////////////////////////////////////////////////
		���_�f�[�^���s�N�Z���̃f�[�^�ɕϊ�����Ƃ��̏����ݒ�
	*/ //////////////////////////////////////////////////////
	vk::PipelineRasterizationStateCreateInfo rasterizationInfo;
	rasterizationInfo.pNext = nullptr;							// �g���p�|�C���^
	rasterizationInfo.flags;									// �\��ς݃t�B�[���h
	rasterizationInfo.cullMode = vk::CullModeFlagBits::eBack;	// �w�ʃJ�����O
	rasterizationInfo.depthBiasClamp = 0.0f;					// �[�x�o�C�A�X�̃N�����v�l
	rasterizationInfo.depthBiasConstantFactor = 0.0f;			// �[�x�o�C�A�X�̒萔�W��
	rasterizationInfo.depthBiasEnable = VK_FALSE;				// �[�x�o�C�A�X�̗L��
	rasterizationInfo.depthBiasSlopeFactor = 0.0f;				// �[�x�o�C�A�X�̃X���[�v�W��
	rasterizationInfo.depthClampEnable = VK_FALSE;				// �[�x�N�����s���O�̗L��
	rasterizationInfo.lineWidth = 1.0f;							// ���̕�
	rasterizationInfo.polygonMode = vk::PolygonMode::eFill;		// �|���S���̕`�惂�[�h
	rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;		// ���X�^���C�U�[�̏����̗L��
	rasterizationInfo.frontFace = vk::FrontFace::eCounterClockwise;	// �|���S���̑O�ʂ̒�`
#pragma endregion rasterizationInfo

	// �}���`�T���v�����O�̐ݒ�
#pragma region multisamplingInfo
	vk::PipelineMultisampleStateCreateInfo multisamplingInfo;
	multisamplingInfo.setRasterizationSamples(vk::SampleCountFlagBits::e1);	// �}���`�T���v�����O���w��
	multisamplingInfo.setSampleShadingEnable(VK_FALSE);		// �T���v���V�F�[�f�B���O�𖳌�
	multisamplingInfo.setMinSampleShading(1.0f);				// �T���v���V�F�[�f�B���O�̍ŏ��䗦
	multisamplingInfo.setPSampleMask(nullptr);				// �T���v���}�X�N���w��i�ʏ��nullptr�j
	multisamplingInfo.setAlphaToCoverageEnable(VK_FALSE);		// �A���t�@�J�o���b�W�𖳌�
	multisamplingInfo.setAlphaToOneEnable(VK_FALSE);			// �A���t�@�l��1�ɌŒ�𖳌�
#pragma endregion multisamplingInfo

	// �J���[�u�����f�B���O�̐ݒ�
#pragma region colorBlendingInfo
// �u�����h�A�^�b�`�����g�X�e�[�g (�ǂ̂悤�Ƀu�����f�B���O���������邩��ݒ�)
	vk::PipelineColorBlendAttachmentState colorState; // �u�����h��Ԃ�������

	// �������݃}�X�N��ݒ�
	// �ǂ̐F�����Ƀu�����f�B���O��K�p���邩���w��
	colorState.colorWriteMask =
		vk::ColorComponentFlagBits::eR |	// ��
		vk::ColorComponentFlagBits::eG |	// ��
		vk::ColorComponentFlagBits::eB |	// ��		
		vk::ColorComponentFlagBits::eA;		// �A���t�@(�����x)

	colorState.blendEnable = VK_TRUE; // �u�����f�B���O��L���ɂ���

	// �u�����f�B���O�̌v�Z�Ɏg�p����v�f��ݒ�
	// �V�����F�ƌÂ��F���ǂ̂悤�ɍ������邩���`
	colorState.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha; // �V�����F�̃A���t�@�l���g�p
	colorState.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha; // �Â��F�̃A���t�@�l�𔽓]

	// �u�����h���Z�����Z�ɐݒ�
	colorState.colorBlendOp = vk::BlendOp::eAdd; // �V�����F�ƌÂ��F�𑫂����킹��
	// �V�����F�̃A���t�@�l * �V�����F + (1 - �V�����F�̃A���t�@�l) * �Â��F

	// �A���t�@�u�����h�̐ݒ�
	colorState.srcAlphaBlendFactor = vk::BlendFactor::eOne;		// �V�����A���t�@�����̂܂܎g�p
	colorState.dstAlphaBlendFactor = vk::BlendFactor::eZero;	// �Â��A���t�@�͖���

	// �A���t�@�u�����h���Z�����Z�ɐݒ�
	colorState.alphaBlendOp = vk::BlendOp::eAdd;				// �V�����A���t�@�ƌÂ��A���t�@�𑫂����킹��
	// 1 * �V�����A���t�@ + 0 * �Â��A���t�@ = �V�����A���t�@

	// �S�̂̃u�����f�B���O�ݒ���܂Ƃ߂�
	vk::PipelineColorBlendStateCreateInfo colorBlendingInfo;	// �u�����h��Ԃ̏�����
	colorBlendingInfo.logicOpEnable = VK_FALSE;				// �_�����Z�𖳌��ɂ���
	colorBlendingInfo.attachmentCount = 1;					// �A�^�b�`�����g�̐���1�ɐݒ�
	colorBlendingInfo.pAttachments = &colorState;				// �ݒ肵���u�����h��Ԃ��w��
#pragma endregion colorBlendingInfo

	// -- DEPTH STENCIL TESTING --
#pragma region depthStencilInfo
	auto depthStencilInfo = CreateDepthStencilStateInfo(true, false);
#pragma endregion depthStencilInfo

	// �_�C�i�~�b�N�����_�����O�̐ݒ�
#pragma region pipelineRenderingInfo
	// 4. Dynamic Rendering�p��PipelineRenderingCreateInfo�̐ݒ�
	vk::PipelineRenderingCreateInfo pipelineRenderingInfo;
	pipelineRenderingInfo.colorAttachmentCount = 1;
	pipelineRenderingInfo.pColorAttachmentFormats = &colorFormat;  // �J���[�A�^�b�`�����g�̃t�H�[�}�b�g
	pipelineRenderingInfo.depthAttachmentFormat = depthFormat;     // �[�x�A�^�b�`�����g�̃t�H�[�}�b�g
#pragma endregion pipelineRenderingInfo


	// ���̓A�Z���u���X�e�[�g�̐ݒ�
#pragma region assemblyStateInfo
	vk::PipelineInputAssemblyStateCreateInfo assemblyStateInfo;
	assemblyStateInfo.setPNext(nullptr);
	//assemblyStateInfo.setFlags((vk::PipelineInputAssemblyStateCreateFlags)0);	// �������̂���0������
	assemblyStateInfo.setTopology(vk::PrimitiveTopology::eTriangleList);   // �g�|���W�[(�O�p�`���X�g)
	assemblyStateInfo.primitiveRestartEnable = VK_FALSE;                 // �v���~�e�B�u�ĊJ�𖳌��ɂ���
#pragma endregion assemblyStateInfo

	m_PipelineInfo.setStages(shaderStageInfos);						// �V�F�[�_�[�X�e�[�W
	m_PipelineInfo.setPVertexInputState(pVertexInputState);	// All the fixed function pipeline states
	m_PipelineInfo.setPInputAssemblyState(&assemblyStateInfo);
	m_PipelineInfo.setPViewportState(&viewportStateInfo);
	m_PipelineInfo.setPDynamicState(nullptr);						//�_�C�i�~�b�N�X�e�[�g�Ƃ�:�p�C�v���C������蒼���Ȃ��Ă��ꕔ����ύX�ł���@�\
	m_PipelineInfo.setPRasterizationState(&rasterizationInfo);
	m_PipelineInfo.setPMultisampleState(&multisamplingInfo);
	m_PipelineInfo.setPColorBlendState(&colorBlendingInfo);
	m_PipelineInfo.setPDepthStencilState(&depthStencilInfo);
	m_PipelineInfo.layout = m_PipelineLayout;
	m_PipelineInfo.pNext = &pipelineRenderingInfo;					// �_�C�i�~�b�N�����_�����O���g�p���邽�߂̒l


	// Create Graphics Pipeline
	auto result = m_pLogicalDevice->createGraphicsPipeline(nullptr, m_PipelineInfo);

	m_Pipeline = result.value;
}

vk::PipelineDepthStencilStateCreateInfo RenderingPipelineCreator::CreateDepthStencilStateInfo(bool depth, bool stencil)
{
	// �[�x����уX�e���V���X�e�[�g�̐ݒ�

	// �[�x����уX�e���V���X�e�[�g�̍쐬����������
	vk::PipelineDepthStencilStateCreateInfo depthStencilCreateInfo;
	depthStencilCreateInfo.depthTestEnable = depth;					// �[�x�e�X�g
	depthStencilCreateInfo.depthWriteEnable = VK_TRUE;				// �[�x�o�b�t�@�ւ̏�������
	depthStencilCreateInfo.depthCompareOp = vk::CompareOp::eLess;	// �[�x��r�I�y���[�V�����̐ݒ�
	depthStencilCreateInfo.depthBoundsTestEnable = VK_FALSE;		// �[�x�o�E���Y�e�X�g
	depthStencilCreateInfo.stencilTestEnable = stencil;				// �X�e���V���e�X�g�𖳌��ɂ���

	if(stencil == true)
	{
		// �X�e���V���I�y���[�V�����̐ݒ��i�X�e���V���e�X�g��L���ɂ����ꍇ�j
		 depthStencilCreateInfo.front.failOp = vk::StencilOp::eKeep;			// �t�����g�ʂł̎��s���̃I�y���[�V����
		 depthStencilCreateInfo.front.passOp = vk::StencilOp::eReplace;			// �t�����g�ʂł̐������̃I�y���[�V����
		 depthStencilCreateInfo.front.depthFailOp = vk::StencilOp::eReplace;	// �t�����g�ʂł̐[�x���s���̃I�y���[�V����
		 depthStencilCreateInfo.front.compareOp = vk::CompareOp::eAlways;		// �t�����g�ʂł̔�r�I�y���[�V����
		 depthStencilCreateInfo.front.compareMask = 0xFF;						// �t�����g�ʂ̔�r�}�X�N
		 depthStencilCreateInfo.front.writeMask = 0xFF;							// �t�����g�ʂ̏������݃}�X�N
		 depthStencilCreateInfo.front.reference = 1;							// �t�����g�ʂ̃��t�@�����X�l
		
		 depthStencilCreateInfo.back.failOp = vk::StencilOp::eKeep;				// �o�b�N�ʂł̎��s���̃I�y���[�V����
		 depthStencilCreateInfo.back.passOp = vk::StencilOp::eReplace;			// �o�b�N�ʂł̐������̃I�y���[�V����
		 depthStencilCreateInfo.back.depthFailOp = vk::StencilOp::eReplace;		// �o�b�N�ʂł̐[�x���s���̃I�y���[�V����
		 depthStencilCreateInfo.back.compareOp = vk::CompareOp::eAlways;		// �o�b�N�ʂł̔�r�I�y���[�V����
		 depthStencilCreateInfo.back.compareMask = 0xFF;						// �o�b�N�ʂ̔�r�}�X�N
		 depthStencilCreateInfo.back.writeMask = 0xFF;							// �o�b�N�ʂ̏������݃}�X�N
		 depthStencilCreateInfo.back.reference = 1;								// �o�b�N�ʂ̃��t�@�����X�l
	}

	return depthStencilCreateInfo;
}

// ���e�͌�Ō��؂���
vk::Format RenderingPipelineCreator::FindSupportedDepthFormat(vk::PhysicalDevice physicalDevice)
{
	vk::Format depthFormats[] = { vk::Format::eD32Sfloat, vk::Format::eD24UnormS8Uint, vk::Format::eD16Unorm };
	for (auto format : depthFormats) {
		vk::FormatProperties props = physicalDevice.getFormatProperties(format);
		if (props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) {
			return format;
		}
	}
	throw std::runtime_error("Failed to find a supported depth format!");
}

vk::PipelineInputAssemblyStateCreateInfo& RenderingPipelineCreator::GetInputAssemblyInfo()
{
	vk::PipelineInputAssemblyStateCreateInfo assemblyStateInfo;
	assemblyStateInfo.setPNext(nullptr);
	//assemblyStateInfo.setFlags((vk::PipelineInputAssemblyStateCreateFlags)0);	// �������̂���0������
	assemblyStateInfo.setTopology(vk::PrimitiveTopology::eTriangleList);   // �g�|���W�[(�O�p�`���X�g)
	assemblyStateInfo.primitiveRestartEnable = VK_FALSE;                 // �v���~�e�B�u�ĊJ�𖳌��ɂ���
	return assemblyStateInfo;
}

//vk::PushConstantRange RenderingPipelineCreator::GetPushConstantModelRange()
//{
//	return vk::PushConstantRange
//	{
//		vk::ShaderStageFlagBits::eVertex,	// �n�������V�F�[�_�[�X�e�[�W
//		0,								    // �n�����f�[�^����ǂ̈ʒu�̃f�[�^�����邩
//		sizeof(Transform)					// �n�������f�[�^�̃T�C�Y
//	};
//}















