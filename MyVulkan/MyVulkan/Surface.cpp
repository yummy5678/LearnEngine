#include "Surface.h"

VkSurfaceKHR VulkanCreate::GetWindowSurface(vk::Instance& instance, GLFWwindow* window)
{
	VkSurfaceKHR c_Surface;
	// �T�[�t�F�X���쐬����i�T�[�t�F�X�쐬���\���̂��쐬���A�T�[�t�F�X�쐬�֐������s���A���ʂ�Ԃ��j
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &c_Surface);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("�T�[�t�F�X�̍쐬�Ɏ��s���܂����I");
	}

	// vk::SurfaceKHR �ɃL���X�g���ĕԂ�
	return c_Surface;

}
