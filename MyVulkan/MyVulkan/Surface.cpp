#include "Surface.h"

vk::UniqueSurfaceKHR SurfaceUtilities::createUnique(vk::Instance& instance, GLFWwindow* window)
{
	VkSurfaceKHR rawSurface;
	// �T�[�t�F�X���쐬����i�T�[�t�F�X�쐬���\���̂��쐬���A�T�[�t�F�X�쐬�֐������s���A���ʂ�Ԃ��j
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &rawSurface);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("�T�[�t�F�X�̍쐬�Ɏ��s���܂����I");
	}

	// �R���X�g���N�^�̑������Őe�ƂȂ�C���X�^���X�̏���n��
	return vk::UniqueSurfaceKHR(rawSurface, instance);

}
