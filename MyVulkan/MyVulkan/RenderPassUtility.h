#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "SwapChainUtility.h"

class RenderpassGenerator : public CGeneratorBase
{
public:
	RenderpassGenerator();
	~RenderpassGenerator();

	void Create(vk::Device logicalDevice, const vk::SurfaceFormatKHR imageFormat);
	void Destroy(vk::Device logicalDevice);

	vk::RenderPass							GetRenderpass();

private:
	vk::Device								m_LogicalDevice;
	vk::AttachmentDescription				m_ColorAttachment;
	vk::AttachmentReference					m_ColorAttachmentRef;
	vk::SubpassDescription					m_Subpass;
	std::vector<vk::SubpassDependency>		m_Dependencies;
	vk::RenderPassCreateInfo				m_RenderPassInfo;
	vk::RenderPass							m_RenderPass;

	vk::RenderPass							CreateRenderpass(vk::Device logicalDevice, const vk::SurfaceFormatKHR imageFormat);
	std::vector<vk::AttachmentDescription>	CreateColorAttachment(const vk::SurfaceFormatKHR imageFormat);
	std::vector<vk::SubpassDescription>		CreateSubpass();
	std::vector<vk::SubpassDependency>		CreateDependencies();

	vk::RenderPassCreateInfo				CreateInfo(	std::vector<vk::AttachmentDescription>& colorAttachment, 
														std::vector<vk::SubpassDescription>& subpass, 
														std::vector<vk::SubpassDependency>& dependencies);

};

// レンダーパスについて
// 描画の処理順序を定義した情報が入るオブジェクト
// アタッチメント、サブパス、サブパス依存関係の3つの要素で構成されている。
// 
// アタッチメントについて
// 作成関数のアタッチメントの値はnullptrにしても構わないが、
// 基本的には何かしらの値が入る
// fomat
// 画像の形式フォーマット。描画対象の画像データと同じものを入れる。
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkFormat.html
// 
// samples
// マルチサンプリングのサンプル数。
// (画像の色の平均を細かく取って、物体の輪郭をぼかす技術。数値が大きいほど計算量が増える)
// 1～64まで2のべき数で好きな値を入れる。描画対象の画像データと同じものを入れる。
// 
// loadOp
// レンダーパス開始時にアタッチメントをどう扱うかを決める。
// VK_ATTACHMENT_LOAD_OP_LOAD		: アタッチメントの既存の内容をそのまま使用します。
// VK_ATTACHMENT_LOAD_OP_CLEAR		: レンダーパス開始時にアタッチメントを指定した値(クリアカラーや深度値)でクリアします。
// VK_ATTACHMENT_LOAD_OP_DONT_CARE	: アタッチメントの初期内容を気にしないことを示します。
// VK_ATTACHMENT_LOAD_OP_NONE_KHR	:アタッチメントの内容を扱わず、特定の初期化操作を行わないことを示します。
// ※デバイスのVK_KHR_maintenance2拡張機能で利用できる
// (VK_ATTACHMENT_LOAD_OP_NONE_EXT)	:アタッチメントの内容を扱わず、特定の初期化操作を行わないことを示します。
// ※デバイスのVK_EXT_attachment_feedback_loop拡張機能で利用できる
// 
// storeOp
// レンダーパスの終了時に描画結果を保持するかどうか決める
// VK_ATTACHMENT_STORE_OP_STORE		:内容をレンダーパス終了時に保存する。出力にデータを引き継ぐ必要がある。
// VK_ATTACHMENT_STORE_OP_DONT_CARE	:アタッチメントの内容を保存しない。レンダーパス終了時にデータは無効になる。
// VK_ATTACHMENT_STORE_OP_NONE		:アタッチメントの内容を保存しない。
// (VK_ATTACHMENT_STORE_OP_NONE_KHR):アタッチメントの内容を保存しない。
// ※デバイスのVK_KHR_maintenance2拡張機能で利用できる
//(VK_ATTACHMENT_STORE_OP_NONE_QCOM):アタッチメントの内容を保存しない。
// ※デバイスのVK_QCOM_render_pass_transform拡張機能で利用できる
// (VK_ATTACHMENT_STORE_OP_NONE_EXT):アタッチメントの内容を保存しない。
// ※デバイスのVK_EXT_attachment_feedback_loop拡張機能で利用できる
// 
// stencilLoadOp
// VK_ATTACHMENT_LOAD_OP_LOAD		:ステンシルバッファの既存の内容をそのまま使用する。
// VK_ATTACHMENT_LOAD_OP_CLEAR		:ステンシルバッファの内容をレンダーパス開始時に指定した値で初期化する
// VK_ATTACHMENT_LOAD_OP_DONT_CARE	:ステンシルバッファを使用しない
// VK_ATTACHMENT_LOAD_OP_NONE_EXT	:
// ※デバイスのVK_EXT_attachment_feedback_loop拡張機能で利用できる
// VK_ATTACHMENT_LOAD_OP_NONE_KHR	:
// ※デバイスのVK_KHR_maintenance2拡張機能で利用できる
// 
// stencilStoreOp
// VK_ATTACHMENT_STORE_OP_STORE		:レンダーパス終了時にステンシルバッファの内容を次に保存する。
// VK_ATTACHMENT_STORE_OP_DONT_CARE	:ステンシルバッファの内容を次に保存しない。
// VK_ATTACHMENT_STORE_OP_NONE		:ステンシルバッファを使用しない。
// VK_ATTACHMENT_STORE_OP_NONE_KHR	:
// ※デバイスのVK_KHR_maintenance2拡張機能で利用できる
// VK_ATTACHMENT_STORE_OP_NONE_QCOM	:
// ※デバイスのVK_QCOM_render_pass_transform拡張機能で利用できる
// VK_ATTACHMENT_STORE_OP_NONE_EXT	:
// ※デバイスのVK_EXT_attachment_feedback_loop拡張機能で利用できる
// 
// initialLayout
// 
// 
// 
// 
// 
// 
// 
// サブパスについて
// 
// 
// 
// サブパス依存関係について
// 
// 
// 
// 
// 
// 







































