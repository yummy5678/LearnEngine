#pragma once
#include <vulkan/vulkan.hpp>
#include "QueueUtility.h"
#include "RenderObject.h"
#include "SemaphoreGenerator.h"
#include "FenceGenerator.h"
#include "VertexBuffer.h"
#include "ImageSet.h"


class DrawCommand
{
public:
	DrawCommand();
	~DrawCommand();

	// 作成関数
	void Create(vk::Device* pLogicalDevice, vk::PhysicalDevice physicalDevice, std::vector<ImageViewSet> imageSet);

	// 破棄関数
	void Destroy();

	void BeginRendering(vk::Pipeline pipeline, vk::Rect2D renderArea);
	void EndRendering();

	// GPU内で画像を描画
	//void DrawFrame(		
	//	vk::PipelineLayout pipelineLayout,
	//	std::vector<vk::DescriptorSet>* descriptorSets,
	//	RenderObject* object,
	//	SceneCamera* camera);


	// メッシュオブジェクトから情報を仕分けて描画コマンドを発行するまでの流れを纏めたもの
	void RenderMesh(
		vk::PipelineLayout pipelineLayout,
		std::vector<vk::DescriptorSet>* descriptorSets,
		VMeshObject* drawMeshes,
		Transform* ObjectTransform);
private:
	vk::Device*						m_pLogicalDevice;
	vk::PhysicalDevice				m_PhysicalDevice;

	// 画像の組が複数枚あるとき(主にスワップチェイン)の描画用インデックス
	uint32_t						m_ImageDrawIndex;

	std::vector<ImageViewSet>			m_ImageSet;

	vk::CommandPool					m_CommandPool;		//コマンドプール
	std::vector<vk::CommandBuffer>	m_CommandBuffers;	//コマンドバッファ

	// 同期オブジェクト
	std::vector<vk::Semaphore>		m_SignalSemaphores;
	std::vector<vk::Semaphore>		m_WaitSemaphores;
	std::vector<vk::Fence>			m_Fences;

	SemaphoreGenerator				m_SemaphoreGenerator;
	FenceGenerator					m_FenceGenerator;



	//コマンドプールの作成
	vk::CommandPool CreateCommandPool(vk::Device* pLogicalDevice, vk::PhysicalDevice physicalDevice);

	//コマンドバッファの作成(コマンドプールの割り当て)
	std::vector<vk::CommandBuffer>	CreateCommandBuffers(vk::Device* pLogicalDevice, uint32_t commandSize, vk::CommandPool commandPool);

	vk::SubmitInfo					CreateSubmitInfo(std::vector<vk::CommandBuffer>& commandBuffers, std::vector<vk::Semaphore>& signalSemaphores, std::vector<vk::Semaphore>& waitSemaphores);
	vk::SubmitInfo					CreateSubmitInfo(vk::CommandBuffer& commandBuffer);

	//uint32_t						AcquireSwapchainNextImage(vk::SwapchainKHR swapchain);









};

