#include "GraphicsPipelineUtility.h"


PipelineGenerator::PipelineGenerator()
{
	m_ClassName = "PipelineGenerator";
}

PipelineGenerator::~PipelineGenerator()
{
}

void PipelineGenerator::Create(vk::Device logicalDevice)
{
	m_bCreated = true;

	// ��������p�ɘ_���f�o�C�X��ێ����Ă���
	m_LogicalDevice = logicalDevice;

	//�p�C�v���C�����C�A�E�g�̍쐬
	m_PipelineLayout = logicalDevice.createPipelineLayout(m_PipelineLayoutInfo);

	//�p�C�v���C���̍쐬
	m_Pipeline = logicalDevice.createGraphicsPipeline(m_PipelineInfo);
		//CreateGraphicsPipeline(logicalDevice, extent, renderPass);
	
}

void PipelineGenerator::CreateInfo(vk::Extent2D extent, vk::RenderPass renderPass, std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfo)
{
	// �r���[�|�[�g�̐ݒ�����쐬
	m_ViewportInfo = CreateViewportStateInfo(extent);

	// ���X�^���C�U�[�̐ݒ�
	m_RasterizerInfo = CreateRasterizerStateInfo();

	// �}���`�T���v�����O�̐ݒ�
	m_MultisamplingInfo = CreateMultisampleStateInfo(vk::SampleCountFlagBits::e1);

	// �J���[�u�����f�B���O�̐ݒ�
	m_ColorBlendInfo = CreateColorBlendingStateInfo();

	// -- DEPTH STENCIL TESTING --
	m_DepthStencilInfo = CreateDepthStencilStateInfo(true,false);



	m_PipelineInfo.setStages(shaderStageInfo);							// �V�F�[�_�[�X�e�[�W
	m_PipelineInfo.setPVertexInputState(&vertexInputInfo);		// All the fixed function pipeline states
	m_PipelineInfo.setPInputAssemblyState(&inputAssemblyInfo);
	m_PipelineInfo.setPViewportState(&m_ViewportInfo);
	m_PipelineInfo.setPDynamicState(nullptr);						//�_�C�i�~�b�N�X�e�[�g�Ƃ�:�p�C�v���C������蒼���Ȃ��Ă��ꕔ����ύX�ł���@�\
	m_PipelineInfo.setPRasterizationState(&m_RasterizerInfo);
	m_PipelineInfo.setPMultisampleState(&m_MultisamplingInfo);
	m_PipelineInfo.setPColorBlendState(&m_ColorBlendInfo);
	m_PipelineInfo.setPDepthStencilState(&m_DepthStencilInfo);
	m_PipelineInfo.layout = m_PipelineLayout;							// �p�C�v���C���ƌ݊��������郌���_�[�p�X
	m_PipelineInfo.renderPass = renderPass;
	m_PipelineInfo.subpass = 0;										// �p�C�v���C���Ŏg�p���郌���_�[ �p�X�̃T�u�p�X

	// Pipeline Derivatives : Can create multiple pipelines that derive from one another for optimisation
	m_PipelineInfo.basePipelineHandle = VK_NULL_HANDLE;	// �h�����̊����̃p�C�v���C��
	m_PipelineInfo.basePipelineIndex = -1;				// �܂��͔h�����Ƃ��č쐬���̃p�C�v���C���̃C���f�b�N�X (�����𓯎��ɍ쐬����ꍇ)


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
	// �V�F�[�_�[�ǂݍ���
	//m_ShaderLoader.LoadShader(logicalDevice, "", "");

	/*auto viewportInfo = CreateViewportStateInfo(extent);*/

	// -- DYNAMIC STATES --
	// Dynamic states to enable
	//std::vector<VkDynamicState> dynamicStateEnables;
	//dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);	// Dynamic Viewport : Can resize in command buffer with vkCmdSetViewport(commandbuffer, 0, 1, &viewport);
	//dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);	// Dynamic Scissor	: Can resize in command buffer with vkCmdSetScissor(commandbuffer, 0, 1, &scissor);

	//// Dynamic State creation info
	//VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	//dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	//dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
	//dynamicStateCreateInfo.pDynamicStates = dynamicStateEnables.data();


	// ���X�^���C�U�[�̐ݒ�
	//vk::PipelineRasterizationStateCreateInfo rasterizerInfo = CreateRasterizerStateInfo();

	// �}���`�T���v�����O�̐ݒ�
	//auto multisamplingInfo = CreateMultisampleStateInfo(vk::SampleCountFlagBits::e1);

	// �J���[�u�����f�B���O�̐ݒ�
	//auto colorBlendingInfo = CreateColorBlendingStateInfo();


	// -- DEPTH STENCIL TESTING --
	//auto depthStencilInfo = CreateDepthStencilStateInfo(true,false);


	auto shaderStage = shader.GetShaderStages();
	// -- GRAPHICS PIPELINE CREATION --
	vk::GraphicsPipelineCreateInfo pipelineCreateInfo;
	pipelineCreateInfo.setStages(shaderStage);							// �V�F�[�_�[�X�e�[�W
	pipelineCreateInfo.setPVertexInputState(&vertexInputInfo);		// All the fixed function pipeline states
	pipelineCreateInfo.setPInputAssemblyState(&inputAssemblyInfo);
	pipelineCreateInfo.setPViewportState(&viewportInfo);
	pipelineCreateInfo.setPDynamicState(nullptr);						//�_�C�i�~�b�N�X�e�[�g�Ƃ�:�p�C�v���C������蒼���Ȃ��Ă��ꕔ����ύX�ł���@�\
	pipelineCreateInfo.setPRasterizationState(&rasterizerInfo);
	pipelineCreateInfo.setPMultisampleState(&multisamplingInfo);
	pipelineCreateInfo.setPColorBlendState(&colorBlendingInfo);
	pipelineCreateInfo.setPDepthStencilState(&depthStencilInfo);
	pipelineCreateInfo.layout = pipelineLayout;							// �p�C�v���C���ƌ݊��������郌���_�[�p�X
	pipelineCreateInfo.renderPass = renderPass;							
	pipelineCreateInfo.subpass = 0;										// �p�C�v���C���Ŏg�p���郌���_�[ �p�X�̃T�u�p�X

	// Pipeline Derivatives : Can create multiple pipelines that derive from one another for optimisation
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;	// �h�����̊����̃p�C�v���C��
	pipelineCreateInfo.basePipelineIndex = -1;				// �܂��͔h�����Ƃ��č쐬���̃p�C�v���C���̃C���f�b�N�X (�����𓯎��ɍ쐬����ꍇ)

	// Create Graphics Pipeline
	result = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Graphics Pipeline!");
	}
}





vk::PipelineLayout PipelineGenerator::CreatePipelineLayout(vk::Device logicalDevice)
{
	// -- PIPELINE LAYOUT --
	std::array<vk::DescriptorSetLayout, 2> descriptorSetLayouts = { descriptorSetLayout, samplerSetLayout };

	vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
	pipelineLayoutCreateInfo.setLayoutCount = descriptorSetLayouts.size();
	pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantModelRange;

	try { return logicalDevice.createPipelineLayout(pipelineLayoutCreateInfo); }
	catch (const std::runtime_error& e) { throw std::runtime_error("�p�C�v���C�����C�A�E�g�̍쐬�Ɏ��s���܂����I"); }
}

inline vk::PipelineViewportStateCreateInfo PipelineGenerator::CreateViewportStateInfo(vk::Extent2D extent)
{
	// �r���[�|�[�g�̏����\�z����
	vk::Viewport viewport;
	viewport.setX(0.0f);									// x���W�̊J�n�ʒu
	viewport.setY(0.0f);									// y���W�̊J�n�ʒu
	viewport.setWidth(static_cast<float>(extent.width));	// �r���[�|�[�g�̕�
	viewport.setHeight(static_cast<float>(extent.height));	// �r���[�|�[�g�̍���
	viewport.setMinDepth(0.0f);								// �t���[���o�b�t�@�̍ŏ��[�x
	viewport.setMaxDepth(1.0f);								// �t���[���o�b�t�@�̍ő�[�x

	// �V�U�[(�r���[�|�[�g�̉�ʂ�؂蔲���̈�)�̏����\�z����
	vk::Rect2D scissor;
	scissor.setOffset({ 0, 0 });							// �g�p����̈�̃I�t�Z�b�g
	scissor.setExtent(extent);								// �g�p����̈�̃T�C�Y

	// �r���[�|�[�g�ƃV�U�[�̏�Ԃ�ݒ肷�邽�߂̍\���̂��쐬
	vk::PipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	viewportStateCreateInfo.setViewportCount(1);		// �r���[�|�[�g�̐�
	viewportStateCreateInfo.setPViewports(&viewport);	// �r���[�|�[�g�̃|�C���^
	viewportStateCreateInfo.setScissorCount(1);			// �V�U�[�̐�
	viewportStateCreateInfo.setPScissors(&scissor);		// �V�U�[�̃|�C���^

	return viewportStateCreateInfo;
}

inline vk::PipelineRasterizationStateCreateInfo PipelineGenerator::CreateRasterizerStateInfo()
{
	// ���X�^���C�[�[�V�����X�e�[�g�̐ݒ�
	vk::PipelineRasterizationStateCreateInfo rasterizationInfo = {};
	rasterizationInfo.setDepthClampEnable(VK_FALSE);										// �[�x�N�����v�𖳌�
	rasterizationInfo.setRasterizerDiscardEnable(VK_FALSE);									// ���X�^���C�U�[��L��
	rasterizationInfo.setPolygonMode(vk::PolygonMode::eFill);								// �h��Ԃ����[�h
	rasterizationInfo.setCullMode(vk::CullModeFlagBits::eBack);								// �w�ʃJ�����O
	rasterizationInfo.setFrontFace(vk::FrontFace::eCounterClockwise);						// �������𔽎��v���ɐݒ�
	rasterizationInfo.setDepthBiasEnable(VK_FALSE);											// �[�x�o�C�A�X�𖳌�
	rasterizationInfo.setDepthBiasConstantFactor(0.0f);										// �[�x�o�C�A�X�̒萔���q
	rasterizationInfo.setDepthBiasClamp(0.0f);												// �[�x�o�C�A�X�̃N�����v�l
	rasterizationInfo.setDepthBiasSlopeFactor(0.0f);										// �[�x�o�C�A�X�̃X���[�v���q

	return rasterizationInfo;
}

inline vk::PipelineMultisampleStateCreateInfo PipelineGenerator::CreateMultisampleStateInfo(vk::SampleCountFlagBits sampleValue)
{
	// �}���`�T���v�����O�̐ݒ�
	vk::PipelineMultisampleStateCreateInfo multisamplingCreateInfo;
	multisamplingCreateInfo.setRasterizationSamples(sampleValue);					// �}���`�T���v�����O���w��
	multisamplingCreateInfo.setSampleShadingEnable(VK_FALSE);						// �T���v���V�F�[�f�B���O�𖳌�
	multisamplingCreateInfo.setMinSampleShading(1.0f);								// �T���v���V�F�[�f�B���O�̍ŏ��䗦
	multisamplingCreateInfo.setPSampleMask(nullptr);								// �T���v���}�X�N���w��i�ʏ��nullptr�j
	multisamplingCreateInfo.setAlphaToCoverageEnable(VK_FALSE);						// �A���t�@�J�o���b�W�𖳌�
	multisamplingCreateInfo.setAlphaToOneEnable(VK_FALSE);							// �A���t�@�l��1�ɌŒ�𖳌�

	return multisamplingCreateInfo;
}

inline vk::PipelineColorBlendStateCreateInfo PipelineGenerator::CreateColorBlendingStateInfo()
{
	// �J���[�u�����f�B���O�̐ݒ�

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
	vk::PipelineColorBlendStateCreateInfo colorBlendingCreateInfo;	// �u�����h��Ԃ̏�����
	colorBlendingCreateInfo.logicOpEnable = VK_FALSE;				// �_�����Z�𖳌��ɂ���
	colorBlendingCreateInfo.attachmentCount = 1;					// �A�^�b�`�����g�̐���1�ɐݒ�
	colorBlendingCreateInfo.pAttachments = &colorState;				// �ݒ肵���u�����h��Ԃ��w��

	return colorBlendingCreateInfo;

}

inline vk::PipelineDepthStencilStateCreateInfo PipelineGenerator::CreateDepthStencilStateInfo(bool depth, bool stencil)
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
	rasterizerCreateInfo.polygonMode = vk::PolygonMode::eFill;		// ���_�Ԃ̃|�C���g�̓h��Ԃ����@
	rasterizerCreateInfo.lineWidth = 1.0f;							// �`�掞�̐��̑���
	rasterizerCreateInfo.cullMode = vk::CullModeFlagBits::eBack;	// �O�p�`�̂ǂ̖ʂ��J�����O���邩
	rasterizerCreateInfo.frontFace = vk::FrontFace::eClockwise;		// �O�ʂ����肷�邽�߂̊�������
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




















