#include "GraphicsPipelineUtility.h"
#include "SwapChainUtility.h"


vk::UniquePipeline GraphicsPipelineUtility::createGraphicsPipeline(vk::Device logicalDevice, vk::Extent2D extent,vk::PipelineLayout pipelineLayout, vk::RenderPass renderPass)
{
	// �V�F�[�_�[��SPIR-V�R�[�h��ǂݍ���
	auto vertexShaderCode = readFile("Shaders/vert.spv");
	auto fragmentShaderCode = readFile("Shaders/frag.spv");

	// �V�F�[�_�[���W���[�����쐬����
	vk::ShaderModule vertexShaderModule   = ShaderUtility::createShaderModule(vertexShaderCode,logicalDevice);
	vk::ShaderModule fragmentShaderModule = ShaderUtility::createShaderModule(fragmentShaderCode,logicalDevice);

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
	vk::PipelineShaderStageCreateInfo shaderStages[] = { vertexShaderCreateInfo, fragmentShaderCreateInfo };


	// -- ���_���� (TODO: ���\�[�X���쐬���ꂽ�璸�_�̐�����ǉ�����) --
	vk::PipelineVertexInputStateCreateInfo vertexInputCreateInfo;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;			// ���_�o�C���f�B���O�̐��� (�f�[�^�̊Ԋu��X�g���C�h���)
	vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;		// ���_�����̐��� (�f�[�^�t�H�[�}�b�g��o�C���h��/��)


	// ���̓A�Z���u���X�e�[�g��ݒ肷��
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.sType = vk::StructureType::ePipelineInputAssemblyStateCreateInfo;
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;         // ���_���A�Z���u�������{�}�`�̎��
	inputAssembly.primitiveRestartEnable = VK_FALSE;                       // "strip" �g�|���W�[���㏑�����ĐV�����v���~�e�B�u���J�n���邱�Ƃ������邩

	// -- �r���[�|�[�g & �V�U�[ --
	// �r���[�|�[�g���̍\���̂��쐬
	vk::Viewport viewport = {};
	viewport.x = 0.0f;									// x �J�n���W
	viewport.y = 0.0f;									// y �J�n���W
	viewport.width = (float)extent.width;				// �r���[�|�[�g�̕�
	viewport.height = (float)extent.height;				// �r���[�|�[�g�̍���
	viewport.minDepth = 0.0f;							// �t���[���o�b�t�@�̍ŏ��[�x
	viewport.maxDepth = 1.0f;							// �t���[���o�b�t�@�̍ő�[�x

	// �V�U�[���̍\���̂��쐬
	vk::Rect2D scissor = {};
	scissor.offset = VkOffset2D{ 0,0 };		// �g�p����̈�̃I�t�Z�b�g
	scissor.extent = extent;				// �g�p����̈�͈̔͂ƃI�t�Z�b�g����J�n

	// �r���[�|�[�g�X�e�[�g�쐬����ݒ肷��
	vk::PipelineViewportStateCreateInfo viewportStateCreateInfo;
	viewportStateCreateInfo.sType = vk::StructureType::ePipelineViewportStateCreateInfo;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;



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


// ���X�^���C�U�[�X�e�[�g��ݒ肷��
	vk::PipelineRasterizationStateCreateInfo rasterizerCreateInfo;
	rasterizerCreateInfo.setDepthClampEnable(VK_FALSE);				// �t���O�����g���ߐ�/���u���ʂ𒴂����ꍇ�ɃN���b�v (�f�t�H���g) �܂��͕��ʂɃN�����v���邩��ύX
	rasterizerCreateInfo.setRasterizerDiscardEnable(VK_FALSE);		// �f�[�^��j�������X�^���C�U���X�L�b�v���邩�ǂ����B�t���[���o�b�t�@�o�͂Ȃ��̃p�C�v���C���ɂ̂ݓK���Ă���
	rasterizerCreateInfo.setPolygonMode(vk::PolygonMode::eFill);	// ���_�Ԃ̃|�C���g�̓h��Ԃ����@
	rasterizerCreateInfo.setLineWidth(1.0f);						// �`�掞�̐��̑���
	rasterizerCreateInfo.setCullMode(vk::CullModeFlagBits::eBack);	// �O�p�`�̂ǂ̖ʂ��J�����O���邩
	rasterizerCreateInfo.setFrontFace(vk::FrontFace::eClockwise);	// �O�ʂ����肷�邽�߂̊�������
	rasterizerCreateInfo.setDepthBiasEnable(VK_FALSE);				// �t���O�����g�ɐ[�x�o�C�A�X��ǉ����邩 (�V���h�E�}�b�s���O�� "�e�A�N�l" ��h���̂ɗL��)



	// �}���`�T���v�����O�X�e�[�g��ݒ肷��
	vk::PipelineMultisampleStateCreateInfo multisamplingCreateInfo;
	multisamplingCreateInfo.setSampleShadingEnable(VK_FALSE);						// �}���`�T���v���V�F�[�f�B���O��L���ɂ��邩�ǂ���
	multisamplingCreateInfo.setRasterizationSamples(vk::SampleCountFlagBits::e1);	// �t���O�����g���ƂɎg�p����T���v����
	multisamplingCreateInfo.setMinSampleShading(1.0f);								// �V�F�[�f�B���O�̍ŏ��T���v����
	multisamplingCreateInfo.setPSampleMask(nullptr);								// �T���v�� �}�X�N
	multisamplingCreateInfo.setAlphaToCoverageEnable(VK_FALSE);						// �A���t�@ �}�X�N
	multisamplingCreateInfo.setAlphaToOneEnable(VK_FALSE);							// �A���t�@ �`���l���̍ő�l


	// -- �u�����f�B���O --
	// �u�����f�B���O�́A�������܂��V�����F�ƌÂ��l���ǂ̂悤�Ƀu�����h���邩�����肷��

// �u�����h�A�^�b�`�����g�X�e�[�g��ݒ肷��
	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR |
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

	vk::PipelineColorBlendStateCreateInfo colourBlendingCreateInfo;
	colourBlendingCreateInfo.pNext;
	colourBlendingCreateInfo.flags;
	colourBlendingCreateInfo.logicOpEnable = VK_FALSE;				// �v�Z�̑���ɘ_�����Z���g�p���邩�ǂ���
	colourBlendingCreateInfo.logicOp;
	colourBlendingCreateInfo.attachmentCount = 1;
	colourBlendingCreateInfo.pAttachments = &colorBlendAttachment;
	colourBlendingCreateInfo.blendConstants;


	//
	// �����ɂ������p�C�v���C�����C�A�E�g����鏈���͕ʂ̏ꏊ�ɋL��
	//



	// -- �[�x�X�e���V���e�X�g --
	// TODO: �[�x�X�e���V���e�X�g�̐ݒ�


	// -- �O���t�B�b�N�X�p�C�v���C���̍쐬 --
	vk::GraphicsPipelineCreateInfo pipelineCreateInfo;

	pipelineCreateInfo.pNext;
	pipelineCreateInfo.flags;
	pipelineCreateInfo.stageCount = 2;// �V�F�[�_�[�X�e�[�W�̐�
	pipelineCreateInfo.pStages = shaderStages;						// �V�F�[�_�[�X�e�[�W�̃��X�g
	pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;	// ���ׂĂ̌Œ�@�\�p�C�v���C���X�e�[�g
	pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
	pipelineCreateInfo.pTessellationState;
	pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	pipelineCreateInfo.pRasterizationState = &rasterizerCreateInfo;
	pipelineCreateInfo.pMultisampleState = &multisamplingCreateInfo;
	pipelineCreateInfo.pDepthStencilState = nullptr;
	pipelineCreateInfo.pColorBlendState = &colourBlendingCreateInfo;
	pipelineCreateInfo.pDynamicState = nullptr;
	pipelineCreateInfo.layout = pipelineLayout;// �p�C�v���C�����g�p����p�C�v���C�����C�A�E�g
	pipelineCreateInfo.renderPass = renderPass;// �p�C�v���C�����݊����̂��郌���_�[�p�X�̐���
	pipelineCreateInfo.subpass = 0;// �p�C�v���C���Ŏg�p����T�u�p�X

	// �p�C�v���C���̔h��: �œK���̂��߂ɑ��݂ɔh�����镡���̃p�C�v���C�����쐬�ł���
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;// �h�����̊����̃p�C�v���C��
	pipelineCreateInfo.basePipelineIndex = -1;// �܂��͍쐬���̃p�C�v���C���̃C���f�b�N�X (�����쐬����ꍇ)

	// �O���t�B�b�N�X�p�C�v���C�����쐬
	vk::UniquePipeline graphicsPipeline = logicalDevice.createGraphicsPipelineUnique(VK_NULL_HANDLE, pipelineCreateInfo).value;
	if (!graphicsPipeline) {
		throw std::runtime_error("�O���t�B�b�N�X�p�C�v���C���̍쐬�Ɏ��s���܂����I");
	}

	//�p�C�v���C���̍쐬��ɕs�v�ɂȂ����V�F�[�_�[���W���[����j��
	//vkDestroyShaderModule(logicalDevice, fragmentShaderModule, nullptr);
	//vkDestroyShaderModule(logicalDevice, vertexShaderModule, nullptr);

	return graphicsPipeline;

}

vk::UniquePipelineLayout GraphicsPipelineUtility::createPipelineLayout(vk::Device logicalDevice)
{
	// -- �p�C�v���C�����C�A�E�g (TODO: �����̃f�B�X�N���v�^�Z�b�g���C�A�E�g��K�p����) --
	vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
	pipelineLayoutCreateInfo.setLayoutCount = 0;
	pipelineLayoutCreateInfo.pSetLayouts = nullptr;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;


	vk::UniquePipelineLayout pipelineLauout = logicalDevice.createPipelineLayoutUnique(pipelineLayoutCreateInfo);

	// �G���[�̏ꍇ���b�Z�[�W�𓊂���
	if (!pipelineLauout)
	{
		throw std::runtime_error("�p�C�v���C�����C�A�E�g�̍쐬�Ɏ��s���܂����I");
	}

	return pipelineLauout;
}
