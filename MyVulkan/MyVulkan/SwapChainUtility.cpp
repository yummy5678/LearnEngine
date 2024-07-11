#include "SwapChainUtility.h"

SwapChainDetails SwapChainUtility::getSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	SwapChainDetails swapChainDetails;

	// -- CAPABILITIES --
	// ����̕����f�o�C�X�ƃT�[�t�F�X�ɑ΂���\�ʂ̃L���r�e�B���擾����
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChainDetails.surfaceCapabilities);

	// -- FORMATS --
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	// �t�H�[�}�b�g���Ԃ��ꂽ�ꍇ�A�t�H�[�}�b�g�̃��X�g���擾����
	if (formatCount != 0)
	{
		swapChainDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChainDetails.formats.data());
	}

	// -- PRESENTATION MODES --
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, nullptr);

	// �v���[���e�[�V�������[�h���Ԃ��ꂽ�ꍇ�A�v���[���e�[�V�������[�h�̃��X�g���擾����
	if (presentationCount != 0)
	{
		swapChainDetails.presentationModes.resize(presentationCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, swapChainDetails.presentationModes.data());
	}

	return swapChainDetails;
}
