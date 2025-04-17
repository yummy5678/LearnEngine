#include "Surface.h"


SurfaceGenerator::SurfaceGenerator(VulkanInitializer& initializer)
{
	glfwInit();
	// �g���@�\�}�l�[�W���[�ɃT�[�t�F�X�̎g�p����g���@�\��ǉ�
	initializer.GetPInstanceExtension()->UseGLFW();
}

SurfaceGenerator::SurfaceGenerator()
{
}

SurfaceGenerator::~SurfaceGenerator()
{
	Cleanup();
}

void SurfaceGenerator::CreateWindowSurface(vk::Instance instance, GLFWwindow* m_pWindow)
{
	m_Instance = instance;

	VkSurfaceKHR c_Surface;
	// �T�[�t�F�X���쐬����i�T�[�t�F�X�쐬���\���̂��쐬���A�T�[�t�F�X�쐬�֐������s���A���ʂ�Ԃ��j
	VkResult result = glfwCreateWindowSurface(instance, m_pWindow, nullptr, &c_Surface);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("�T�[�t�F�X�̍쐬�Ɏ��s���܂����I");
	}

	m_Surface = c_Surface;
}

VkSurfaceKHR SurfaceGenerator::GetSurface()
{
	return m_Surface;
}

vk::SurfaceCapabilitiesKHR SurfaceGenerator::GetCapabilities(vk::PhysicalDevice physicalDevice)
{
	return physicalDevice.getSurfaceCapabilitiesKHR(m_Surface);
}

std::vector<vk::SurfaceFormatKHR> SurfaceGenerator::GetFomats(vk::PhysicalDevice physicalDevice)
{
	return physicalDevice.getSurfaceFormatsKHR(m_Surface);
}

void SurfaceGenerator::Cleanup()
{
	if (m_Instance == VK_NULL_HANDLE) return;

	if (m_Surface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		m_Surface = VK_NULL_HANDLE;
	}

	m_Instance = VK_NULL_HANDLE;
}

//std::vector<const char*> SurfaceGenerator::GetGLFWSurfaceExtensions()
//{
//	std::cout << m_ClassName << "GLFW�g���@�\�̃��X�g�̎󂯎��" << std::endl;
//
//	// �C���X�^���X�g���@�\�̃��X�g���쐬����
//	std::vector<const char*> instanceExtensions;
//
//	// �C���X�^���X���g�p����g���@�\��ݒ肷��
//	uint32_t glfwExtensionCount = 0;	// GLFW�͕����̊g���@�\��v������ꍇ������
//	const char** glfwExtensions;		// �g���@�\��C������̔z��Ƃ��ēn����邽�߁A�|�C���^(�z��)�̃|�C���^(C������)���K�v
//
//	// GLFW�̊g���@�\���擾����
//	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//	// GLFW�̊g���@�\�����X�g�ɒǉ�����
//	for (size_t i = 0; i < glfwExtensionCount; i++)
//	{
//		instanceExtensions.push_back(glfwExtensions[i]);
//		std::cout << "�g���@�\���F" << glfwExtensions[i] << std::endl;
//	}
//
//	return instanceExtensions;
//}
