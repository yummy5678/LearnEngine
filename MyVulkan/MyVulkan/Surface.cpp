#include "Surface.h"


SurfaceGenerator::SurfaceGenerator()
{
	m_ClassName = "SurfaceGenerator";
}

SurfaceGenerator::~SurfaceGenerator()
{
	Destroy(m_Instance);
}

void SurfaceGenerator::CreateWindowSurface(vk::Instance instance, GLFWwindow* window)
{
	m_bCreated = true;
	m_Instance = instance;

	VkSurfaceKHR c_Surface;
	// �T�[�t�F�X���쐬����i�T�[�t�F�X�쐬���\���̂��쐬���A�T�[�t�F�X�쐬�֐������s���A���ʂ�Ԃ��j
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &c_Surface);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("�T�[�t�F�X�̍쐬�Ɏ��s���܂����I");
	}

	m_Surface = c_Surface;
}

VkSurfaceKHR SurfaceGenerator::GetSurface()
{
	CheckCreated();
	return m_Surface;
}

vk::SurfaceCapabilitiesKHR SurfaceGenerator::GetCapabilities(vk::PhysicalDevice physicalDevice)
{
	CheckCreated();
	return physicalDevice.getSurfaceCapabilitiesKHR(m_Surface);
}

void SurfaceGenerator::Destroy(vk::Instance instance)
{
	vkDestroySurfaceKHR(instance, m_Surface, nullptr);
}
