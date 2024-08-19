#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "QueueUtility.h"
#include "GraphicsDefine.h"
#include "DeviceExtensionManager.h"
//#include "Utilities.h"

const int MAX_FRAME_DRAWS = 3;


//struct SwapchainImage 
//{
//	vk::Image		image;		// Vulkan�̒��ň����摜
//	vk::ImageView	imageView;	// �摜�r���[
//};

struct SwapChainDetails {
	vk::SurfaceCapabilitiesKHR			surfaceCapabilities;	// Surface properties, e.g. image size/extent
	std::vector<vk::SurfaceFormatKHR>	formats;				// Surface image formats, e.g. RGBA and size of each colour
	std::vector<vk::PresentModeKHR>		presentationModes;		// How images should be presented to screen
};


class SwapchainGenerator : public CGeneratorBase
{
public:
	SwapchainGenerator(CDeviceExtensionManager& deviceExtensionManager);
	~SwapchainGenerator();

	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	void Destroy(vk::Device logicalDevice);

	vk::SwapchainKHR			GetSwapchain();
	vk::Extent2D				Get2DExtent();
	vk::SurfaceFormatKHR		GetSwapSurfaceFormat();
	vk::SwapchainCreateInfoKHR	GetSwapchainInfo();
	//std::vector<SwapchainImage>	GetSwapChainImages();

private:
	vk::Device m_LogicalDevice;
	vk::SwapchainCreateInfoKHR			m_SwapchainInfo;
	vk::SwapchainKHR					m_Swapchain;

	//vk::SurfaceCapabilitiesKHR		m_SurfaceCapabilities;
	vk::SurfaceFormatKHR				m_SurfaceFormat;
	vk::Extent2D						m_Extent;
	vk::PresentModeKHR					m_PresentMode;

	//std::vector<SwapchainImage>			m_Images;
	


	// �X���b�v�`�F�C���̍쐬�֐�
	vk::SwapchainCreateInfoKHR CreateSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	//std::vector<SwapchainImage> CreateSwapChainImages(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::SwapchainKHR swapchain);

	//vk::ImageView CreateImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
	// �T�[�t�F�X�̋@�\���擾
	//vk::SurfaceCapabilitiesKHR GetSurfaceCapabilities(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	// �X���b�v�`�F�C���̃t�H�[�}�b�g��I������֐�
	vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	// �X���b�v�`�F�C���̃v���[���g���[�h��I������֐�
	vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
	// �X���b�v�`�F�C���̃G�N�X�e���g��I������֐�
	vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

	
	std::vector<vk::SurfaceFormatKHR> GetSurfaceFormats(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	std::vector<vk::PresentModeKHR> GetPresentModes(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
};