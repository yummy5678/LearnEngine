#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"

class SynchronizationGenerator : public CGeneratorBase
{
public:
	SynchronizationGenerator();
	~SynchronizationGenerator();

	void Create(vk::Device logicalDevice);
	void Destroy(vk::Device logicalDevice);

	std::vector<vk::Semaphore>	GetImageAvailable();
	std::vector<vk::Semaphore>	GetRenderFinished();
	std::vector<vk::Fence>		GetDrawFences();

private:
	std::vector<vk::Semaphore>	m_ImageAvailable;
	std::vector<vk::Semaphore>	m_RenderFinished;
	std::vector<vk::Fence>		m_DrawFences;


	void CreateSynchronisation(vk::Device logicalDevice);


};

