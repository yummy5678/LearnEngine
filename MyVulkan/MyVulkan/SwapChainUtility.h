#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "QueueUtility.h"
#include "GraphicsDefine.h"
//#include "Utilities.h"

const int MAX_FRAME_DRAWS = 3;

struct UniqueSwapchainImage 
{
	vk::UniqueImage		image;		// Vulkanの中で扱う画像
	vk::UniqueImageView	imageView;	// 画像ビュー
};

struct SwapchainImage 
{
	vk::Image		image;		// Vulkanの中で扱う画像
	vk::ImageView	imageView;	// 画像ビュー

	//オペレーターを用意して受け渡しを簡単にする
	//SwapchainImage& operator=(const UniqueSwapchainImage& unique)
	//{
	//	this->image = unique.image.get();
	//	this->imageView = unique.imageView.get();
	//	return *this;
	//}
};



struct SwapChainDetails {
	vk::SurfaceCapabilitiesKHR			surfaceCapabilities;	// Surface properties, e.g. image size/extent
	std::vector<vk::SurfaceFormatKHR>	formats;				// Surface image formats, e.g. RGBA and size of each colour
	std::vector<vk::PresentModeKHR>		presentationModes;		// How images should be presented to screen
};

namespace VulkanUtility
{
	//SwapChainDetails getSwapChainDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);


	//// スワップチェインのフォーマットを選択する関数
	//vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

	//// スワップチェインのプレゼントモードを選択する関数
	//vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

	//// スワップチェインのエクステントを選択する関数
	//vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

	//// サーフェスの機能を取得
	//vk::SurfaceCapabilitiesKHR getSurfaceCapabilities(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	//// サーフェスフォーマットのリストを取得
	//std::vector<vk::SurfaceFormatKHR> getSurfaceFormats(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	//std::vector<vk::PresentModeKHR> getPresentModes(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);


	//std::vector<SwapchainImage> createSwapChainImages(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::SwapchainKHR swapchain);

	//vk::ImageView createImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);

	//std::vector<vk::Framebuffer> createFramebuffers(vk::Device logicalDevice, std::vector<SwapchainImage> swapChainImages, vk::RenderPass renderPass, vk::Extent2D extent);

};

class SwapchainGenerator : public CGeneratorBase
{
public:
	SwapchainGenerator();
	~SwapchainGenerator();

	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	void Destroy(vk::Device logicalDevice);

	vk::SwapchainKHR			GetSwapchain();
	vk::Extent2D				Get2DExtent();
	vk::SurfaceFormatKHR		GetSwapSurfaceFormat();
	vk::SwapchainCreateInfoKHR	GetSwapchainInfo();
	std::vector<SwapchainImage>	GetSwapChainImages();

private:
	vk::Device m_LogicalDevice;
	vk::SwapchainCreateInfoKHR			m_SwapchainInfo;
	vk::SwapchainKHR					m_Swapchain;

	//vk::SurfaceCapabilitiesKHR		m_SurfaceCapabilities;
	vk::SurfaceFormatKHR				m_SurfaceFormat;
	vk::Extent2D						m_Extent;
	vk::PresentModeKHR					m_PresentMode;

	std::vector<SwapchainImage>			m_Images;
	


	// スワップチェインの作成関数
	vk::SwapchainCreateInfoKHR CreateSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

	std::vector<SwapchainImage> CreateSwapChainImages(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::SwapchainKHR swapchain);

	vk::ImageView CreateImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
	// サーフェスの機能を取得
	//vk::SurfaceCapabilitiesKHR GetSurfaceCapabilities(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	// スワップチェインのフォーマットを選択する関数
	vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	// スワップチェインのプレゼントモードを選択する関数
	vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
	// スワップチェインのエクステントを選択する関数
	vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

	
	std::vector<vk::SurfaceFormatKHR> GetSurfaceFormats(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
	std::vector<vk::PresentModeKHR> GetPresentModes(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
};