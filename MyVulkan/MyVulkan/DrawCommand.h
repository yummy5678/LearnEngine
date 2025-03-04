#pragma once
#include <vulkan/vulkan.hpp>
#include "QueueUtility.h"
#include "RenderObject.h"
#include "SemaphoreGenerator.h"
#include "FenceGenerator.h"
#include "VertexBuffer.h"
#include "ImageSet.h"
#include "RendererBase.h"
#include "SwapchainRenderer.h"


class DrawCommand
{
public:
	DrawCommand();
	~DrawCommand();

	// 作成関数
	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);

	void Destroy();

	vk::CommandBuffer GetBuffer();

	void BeginRendering(RenderingImageSet* imageSet, vk::Semaphore imageAvableSemaphore, vk::Rect2D renderArea);
	void EndRendering(vk::ImageLayout newImageLayout);

	vk::Semaphore GetSignalSemaphore();
	vk::Fence GetFence();

	//uint32_t GetCurrentIndex();
	void WaitFence();

	// GPU内で画像を描画
	//void DrawFrame(		
	//	vk::PipelineLayout pipelineLayout,
	//	std::vector<vk::DescriptorSet>* descriptorSets,
	//	RenderObject* object,
	//	SceneCamera* camera);


	// メッシュオブジェクトから情報を仕分けて描画コマンドを発行するまでの流れを纏めたもの
	//void RenderMesh(
	//	vk::Pipeline pipeline, 
	//	vk::PipelineLayout pipelineLayout,
	//	std::vector<vk::DescriptorSet>* descriptorSets,
	//	VMeshObject* drawMeshes,
	//	Transform* ObjectTransform);
private:
	vk::Device						m_LogicalDevice;
	vk::PhysicalDevice				m_PhysicalDevice;
	vk::SwapchainKHR				m_Swapchain;

	QueueFamilySelector				m_QueueSelector;

	// 画像の組が複数枚あるとき(主にスワップチェイン)の描画用インデックス
	// 今から描画するイメージインデックス
	//uint32_t						m_CurrentIndex;	// 用途 : フェンス、セマフォ
	//// 次フレームで描画するイメージインデックス
	//uint32_t						m_NextIndex;	// 用途 : コマンド、画像



	RenderingImageSet*				m_ImageSet;

	vk::CommandPool					m_CommandPool;		//コマンドプール
	vk::CommandBuffer				m_CommandBuffers;	//コマンドバッファ

	// 同期オブジェクト
	vk::Semaphore					m_RenderFinishedSemaphores;

	// 描画処理の完了を知らせるためのセマフォ
	vk::Semaphore					m_ImageAvailableSemaphores;
	vk::Fence						m_Fences;

	//SemaphoreGenerator				m_SemaphoreGenerator;
	//FenceGenerator					m_FenceGenerator;

	void CreateSemaphore(vk::Semaphore& semaphore);
	void CreateFence(vk::Fence& fence);

	//コマンドプールの作成
	void CreateCommandPool();

	//コマンドバッファの作成(コマンドプールの割り当て)
	void CreateCommandBuffers(uint32_t commandSize, vk::CommandPool commandPool);

	vk::SubmitInfo CreateSubmitInfo(std::vector<vk::PipelineStageFlags>* waitStages);

	void ImageMemoryBarrier(vk::Image& image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);





};

