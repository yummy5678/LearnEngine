#include "GraphicsPipelineUtility.h"
#include "SwapChainUtility.h"


PipelineGenerator::PipelineGenerator()
{
	m_ClassName = "PipelineGenerator";
}

PipelineGenerator::~PipelineGenerator()
{
}

void PipelineGenerator::Create(vk::Device logicalDevice, vk::Extent2D extent, vk::RenderPass renderPass)
{
	m_bCreated = true;

	// ��������p�ɘ_���f�o�C�X��ێ����Ă���
	m_LogicalDevice = logicalDevice;

	//�p�C�v���C�����C�A�E�g�̍쐬
	m_PipelineLayout = CreatePipelineLayout(logicalDevice);

	//�p�C�v���C���̍쐬
	m_Pipeline = CreateGraphicsPipeline(logicalDevice, extent, renderPass);
	
}

void PipelineGenerator::Destroy(vk::Device logicalDevice)
{
	logicalDevice.destroyPipelineLayout(m_PipelineLayout);
	logicalDevice.destroyPipeline(m_Pipeline);

	//�p�C�v���C���̍쐬��ɕs�v�ɂȂ����V�F�[�_�[���W���[����j��
	//vkDestroyShaderModule(logicalDevice, fragmentShaderModule, nullptr);
	//vkDestroyShaderModule(logicalDevice, vertexShaderModule, nullptr);
}

vk::Pipeline PipelineGenerator::GetPipeline()
{
	CheckCreated();
	return m_Pipeline;
}

vk::PipelineLayout PipelineGenerator::GetPipelineLayout()
{
	CheckCreated();
	return m_PipelineLayout;
}


vk::Pipeline PipelineGenerator::CreateGraphicsPipeline(vk::Device logicalDevice, vk::Extent2D extent, vk::RenderPass renderPass)
{
	//�V�F�[�_�[�X�e�[�W�̍쐬
	auto shaderStageInfos = GetShaderStageInfo(logicalDevice);

	//vertexInputCreateInfo�̍쐬
	auto vertexInputCreateInfo = GetVertexInputStateInfo();

	//���̓A�Z���u���X�e�[�g�̍쐬
	auto inputAssemblyInfo = GetInputAssemblyStateInfo();

	//�r���[�|�[�g�X�e�[�g�̍쐬
	m_viewportGenerator.Create(extent);
	auto viewportStateCreateInfo = m_viewportGenerator.GetCreateInfoPointer();

	//���X�^���C�U�[�X�e�[�g�̍쐬
	auto rasterizerCreateInfo = GetRasterizationStateInfo();

	// -- �_�C�i�~�b�N�X�e�[�g --
	// �L���ɂ���_�C�i�~�b�N�X�e�[�g
	//std::vector<VkDynamicState> dynamicStateEnables;
	//dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);	// �_�C�i�~�b�N�r���[�|�[�g: vkCmdSetViewport(commandbuffer, 0, 1, &viewport); �ŃR�}���h�o�b�t�@���Ń��T�C�Y�\
	//dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);	// �_�C�i�~�b�N�V�U�[: vkCmdSetScissor(commandbuffer, 0, 1, &scissor); �ŃR�}���h�o�b�t�@���Ń��T�C�Y�\

	//// �_�C�i�~�b�N�X�e�[�g�̍쐬���
	//VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	//dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	//dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
	//dynamicStateCreateInfo.pDynamicStates = dynamicStateEnables.data();

	//�}���`�T���v�����O�X�e�[�g�̍쐬
	auto multisamplingInfo = GetMultisampleStateInfo();

	//�u�����h�X�e�[�g�̍쐬
	auto colorBlendAttachment = GetColorBlendAttachmentState();
	auto colorBlendCreateInfo = GetColorBlendStateInfo(&colorBlendAttachment);


	// -- �O���t�B�b�N�X�p�C�v���C���̍쐬 --
	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.pNext;
	pipelineInfo.flags;
	pipelineInfo.stageCount			= shaderStageInfos.size();	// �V�F�[�_�[�X�e�[�W�̐�
	pipelineInfo.pStages			= shaderStageInfos.data();	// �V�F�[�_�[�X�e�[�W�̃��X�g
	pipelineInfo.pVertexInputState	= &vertexInputCreateInfo;		// ���ׂĂ̌Œ�@�\�p�C�v���C���X�e�[�g
	pipelineInfo.pInputAssemblyState= &inputAssemblyInfo;
	pipelineInfo.pTessellationState	= nullptr;					//���쐬
	pipelineInfo.pViewportState		= viewportStateCreateInfo;
	pipelineInfo.pRasterizationState= &rasterizerCreateInfo;
	pipelineInfo.pMultisampleState	= &multisamplingInfo;
	pipelineInfo.pDepthStencilState	= nullptr;					//���쐬
	pipelineInfo.pColorBlendState	= &colorBlendCreateInfo;
	pipelineInfo.pDynamicState		= nullptr;					//���쐬
	pipelineInfo.layout				= m_PipelineLayout;			// �p�C�v���C�����g�p����p�C�v���C�����C�A�E�g
	pipelineInfo.renderPass			= renderPass;				// �p�C�v���C�����݊����̂��郌���_�[�p�X�̐���
	pipelineInfo.subpass			= 0;						// �p�C�v���C���Ŏg�p����T�u�p�X

	// �p�C�v���C���̔h��: �œK���̂��߂ɑ��݂ɔh�����镡���̃p�C�v���C�����쐬�ł���
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;// �h�����̊����̃p�C�v���C��
	pipelineInfo.basePipelineIndex = -1;// �܂��͍쐬���̃p�C�v���C���̃C���f�b�N�X (�����쐬����ꍇ)

	// �O���t�B�b�N�X�p�C�v���C�����쐬
	auto result = logicalDevice.createGraphicsPipeline(nullptr, pipelineInfo);
	if (result.result != vk::Result::eSuccess)
	{
		throw std::runtime_error("�O���t�B�b�N�X�p�C�v���C���̍쐬�Ɏ��s���܂����I");
	}
	return result.value;
}

vk::PipelineLayout PipelineGenerator::CreatePipelineLayout(vk::Device logicalDevice)
{
	// -- �p�C�v���C�����C�A�E�g (TODO: �����̃f�B�X�N���v�^�Z�b�g���C�A�E�g��K�p����) --
	vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	try
	{
		return logicalDevice.createPipelineLayout(pipelineLayoutInfo);
	}
	catch (const std::runtime_error& e)
	{
		throw std::runtime_error("�p�C�v���C�����C�A�E�g�̍쐬�Ɏ��s���܂����I");
	}
}

std::vector<vk::PipelineShaderStageCreateInfo> PipelineGenerator::GetShaderStageInfo(vk::Device logicalDevice)
{
	// �V�F�[�_�[��SPIR-V�R�[�h��ǂݍ���
	auto vertexShaderCode = readFile("Shaders/vert.spv");
	auto fragmentShaderCode = readFile("Shaders/frag.spv");

	// �V�F�[�_�[���W���[�����쐬����
	vk::ShaderModule vertexShaderModule = ShaderUtility::createShaderModule(vertexShaderCode, logicalDevice);
	vk::ShaderModule fragmentShaderModule = ShaderUtility::createShaderModule(fragmentShaderCode, logicalDevice);

	// -- �V�F�[�_�[�X�e�[�W�쐬��� --
	// ���_�V�F�[�_�[�X�e�[�W�̍쐬���
	vk::PipelineShaderStageCreateInfo vertexShaderCreateInfo;
	vertexShaderCreateInfo.stage = vk::ShaderStageFlagBits::eVertex;		// �V�F�[�_�[�X�e�[�W�̎��
	vertexShaderCreateInfo.module = vertexShaderModule;						// �g�p����V�F�[�_�[���W���[��
	vertexShaderCreateInfo.pName = "main";									// �G���g���[�|�C���g

	// �t���O�����g�V�F�[�_�[�X�e�[�W�̍쐬���
	vk::PipelineShaderStageCreateInfo fragmentShaderCreateInfo;
	fragmentShaderCreateInfo.stage = vk::ShaderStageFlagBits::eFragment;        // �V�F�[�_�[�X�e�[�W�̎��
	fragmentShaderCreateInfo.module = fragmentShaderModule;						// �g�p����V�F�[�_�[���W���[��
	fragmentShaderCreateInfo.pName = "main";									// �G���g���[�|�C���g

	// �V�F�[�_�[�X�e�[�W�쐬����z��Ɋi�[
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = { vertexShaderCreateInfo, fragmentShaderCreateInfo };

	return shaderStages;
}

vk::PipelineVertexInputStateCreateInfo PipelineGenerator::GetVertexInputStateInfo()
{
	// -- ���_���� (TODO: ���\�[�X���쐬���ꂽ�璸�_�̐�����ǉ�����) --
	vk::PipelineVertexInputStateCreateInfo vertexInputCreateInfo;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;			// ���_�o�C���f�B���O�̐��� (�f�[�^�̊Ԋu��X�g���C�h���)
	vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;		// ���_�����̐��� (�f�[�^�t�H�[�}�b�g��o�C���h��/��)

	return vertexInputCreateInfo;
}

vk::PipelineInputAssemblyStateCreateInfo PipelineGenerator::GetInputAssemblyStateInfo()
{
	// ���̓A�Z���u���X�e�[�g��ݒ肷��
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.sType = vk::StructureType::ePipelineInputAssemblyStateCreateInfo;
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;         // ���_���A�Z���u�������{�}�`�̎��
	inputAssembly.primitiveRestartEnable = VK_FALSE;                       // "strip" �g�|���W�[���㏑�����ĐV�����v���~�e�B�u���J�n���邱�Ƃ������邩
	
	return inputAssembly;
}

vk::PipelineRasterizationStateCreateInfo PipelineGenerator::GetRasterizationStateInfo()
{
	// ���X�^���C�U�[�X�e�[�g��ݒ肷��
	vk::PipelineRasterizationStateCreateInfo rasterizerCreateInfo;
	rasterizerCreateInfo.depthClampEnable = VK_FALSE;				// �t���O�����g���ߐ�/���u���ʂ𒴂����ꍇ�ɃN���b�v (�f�t�H���g) �܂��͕��ʂɃN�����v���邩��ύX
	rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;		// �f�[�^��j�������X�^���C�U���X�L�b�v���邩�ǂ����B�t���[���o�b�t�@�o�͂Ȃ��̃p�C�v���C���ɂ̂ݓK���Ă���
	rasterizerCreateInfo.polygonMode = vk::PolygonMode::eFill;	// ���_�Ԃ̃|�C���g�̓h��Ԃ����@
	rasterizerCreateInfo.lineWidth = 1.0f;						// �`�掞�̐��̑���
	rasterizerCreateInfo.cullMode = vk::CullModeFlagBits::eBack;	// �O�p�`�̂ǂ̖ʂ��J�����O���邩
	rasterizerCreateInfo.frontFace = vk::FrontFace::eClockwise;	// �O�ʂ����肷�邽�߂̊�������
	rasterizerCreateInfo.depthBiasEnable = VK_FALSE;				// �t���O�����g�ɐ[�x�o�C�A�X��ǉ����邩 (�V���h�E�}�b�s���O�� "�e�A�N�l" ��h���̂ɗL��)

	return rasterizerCreateInfo;
}

vk::PipelineMultisampleStateCreateInfo PipelineGenerator::GetMultisampleStateInfo()
{
	// �}���`�T���v�����O�X�e�[�g��ݒ肷��
	vk::PipelineMultisampleStateCreateInfo multisamplingCreateInfo;
	multisamplingCreateInfo.setSampleShadingEnable(VK_FALSE);						// �}���`�T���v���V�F�[�f�B���O��L���ɂ��邩�ǂ���
	multisamplingCreateInfo.setRasterizationSamples(vk::SampleCountFlagBits::e1);	// �t���O�����g���ƂɎg�p����T���v����
	multisamplingCreateInfo.setMinSampleShading(1.0f);								// �V�F�[�f�B���O�̍ŏ��T���v����
	multisamplingCreateInfo.setPSampleMask(nullptr);								// �T���v�� �}�X�N
	multisamplingCreateInfo.setAlphaToCoverageEnable(VK_FALSE);						// �A���t�@ �}�X�N
	multisamplingCreateInfo.setAlphaToOneEnable(VK_FALSE);							// �A���t�@ �`���l���̍ő�l

	return multisamplingCreateInfo;
}


std::vector<vk::PipelineColorBlendAttachmentState> PipelineGenerator::GetColorBlendAttachmentState()
{
	// �u�����h�A�^�b�`�����g�X�e�[�g��ݒ肷��
	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.colorWriteMask = 
		vk::ColorComponentFlagBits::eR |
		vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB |
		vk::ColorComponentFlagBits::eA;
	colorBlendAttachment.blendEnable = VK_TRUE; // �u�����f�B���O��L���ɂ���

	// �u�����h�A���t�@�̌v�Z����ݒ肷��
	colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
	colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
	colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;

	// �A���t�@�u�����h�̌v�Z����ݒ肷��
	colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
	colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
	colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;
	// �v��: (1 * �V�����A���t�@) + (0 * �Â��A���t�@) = �V�����A���t�@

	return std::vector<vk::PipelineColorBlendAttachmentState>{colorBlendAttachment};
}

vk::PipelineColorBlendStateCreateInfo PipelineGenerator::GetColorBlendStateInfo(std::vector<vk::PipelineColorBlendAttachmentState>* pAttachments)
{
	vk::PipelineColorBlendStateCreateInfo colorBlendingInfo;
	colorBlendingInfo.pNext;
	colorBlendingInfo.flags;
	colorBlendingInfo.logicOpEnable = VK_FALSE;				// �v�Z�̑���ɘ_�����Z���g�p���邩�ǂ���
	colorBlendingInfo.logicOp;
	colorBlendingInfo.attachmentCount = pAttachments->size();
	colorBlendingInfo.pAttachments = pAttachments->data();
	colorBlendingInfo.blendConstants;

	return colorBlendingInfo;
}
