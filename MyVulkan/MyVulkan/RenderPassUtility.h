#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "SwapChainUtility.h"

class RenderpassGenerator : public CGeneratorBase
{
public:
	RenderpassGenerator();
	~RenderpassGenerator();

	void Create(vk::Device logicalDevice, vk::Format imageFomat);
	void Destroy();

	vk::RenderPass							GetRenderpass();

private:
	vk::Device								m_LogicalDevice;
	vk::AttachmentDescription				m_ColorAttachment;

	// アタッチメントリファレンス(アタッチメントの参照情報)
	vk::AttachmentReference					m_InputAttachmentRef;
	vk::AttachmentReference					m_ColorAttachmentRef;
	vk::AttachmentReference					m_ResolveAttachmentRef;
	vk::AttachmentReference					m_DepthStencilAttachmentRef;
	vk::AttachmentReference					m_PreserveAttachmentRef;

	vk::SubpassDescription					m_Subpass;
	std::vector<vk::SubpassDependency>		m_Dependencies;
	vk::RenderPassCreateInfo				m_RenderPassInfo;
	vk::RenderPass							m_RenderPass;

	// プライベート関数
	vk::RenderPass							CreateRenderpass(vk::Device logicalDevice, vk::Format imageFomat);

};

// レンダーパスについて
// 描画の処理手順を定義するオブジェクト
// アタッチメント、サブパス、サブパス依存関係の3つの要素で構成されている。

// アタッチメント(画像データ)について
// レンダーパスで扱う画像データを定義するための構造体

// fomat
// 画像の形式フォーマット。(色の種類、ビット深度)
// 描画対象の画像データと同じものを入れる。
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkFormat.html

// samples
// マルチサンプリングのサンプル数。
// (画像の色の平均を細かく取って、物体の輪郭をぼかす技術。数値が大きいほど計算量が増える)
// 1～64まで2のべき数で好きな値を入れる。描画対象の画像データと同じものを入れる。

// loadOpとは
// 描画を始める前の画像をどう扱うかの情報。
// 画像を綺麗にリセットして書き直すか、
// 前に書いた情報に上から書くか、といった内容。
// 
// VK_ATTACHMENT_LOAD_OP_LOAD		: 前回の描画で入っていた画像データをそのまま使う。
// VK_ATTACHMENT_LOAD_OP_CLEAR		: 値を指定した値(通常は0)で初期化する。
// VK_ATTACHMENT_LOAD_OP_DONT_CARE	: 前回の描画で入っていた画像データを無視する。初期状態が関係ない時に使う。
// VK_ATTACHMENT_LOAD_OP_NONE_KHR	: 前回の描画で入っていた画像データを無視する。(詳細は知らん)
// ※デバイスのVK_KHR_maintenance2拡張機能で利用できる
// (VK_ATTACHMENT_LOAD_OP_NONE_EXT)	: 前回の描画で入っていた画像データを無視する。(詳細は知らん)
// ※デバイスのVK_EXT_attachment_feedback_loop拡張機能で利用できる

// storeOpとは
// 描画した画像データをどう扱うかの情報。
// レンダーパス終了時に内容を保存するか、しないかといった内容。
// 
// VK_ATTACHMENT_STORE_OP_STORE		:描画結果を保存する
// VK_ATTACHMENT_STORE_OP_DONT_CARE	:描画結果を保存しない。
// VK_ATTACHMENT_STORE_OP_NONE		:アタッチメントの内容を保存しない。(詳細は知らん)
// VK_ATTACHMENT_STORE_OP_NONE_KHR  :アタッチメントの内容を保存しない。(詳細は知らん)
// ※デバイスのVK_KHR_maintenance2拡張機能で利用できる
// VK_ATTACHMENT_STORE_OP_NONE_QCOM :アタッチメントの内容を保存しない。(詳細は知らん)
// ※デバイスのVK_QCOM_render_pass_transform拡張機能で利用できる
// VK_ATTACHMENT_STORE_OP_NONE_EXT  :アタッチメントの内容を保存しない。(詳細は知らん)
// ※デバイスのVK_EXT_attachment_feedback_loop拡張機能で利用できる

// ステンシルとは
// 3D描画において、マスク処理のような
// 描画する場所や、しない場所の画像情報

// (イメージ)レイアウトとは
// 画像データが描画される前後で、どのようにメモリに配置、最適化されるかの定義情報。
// 
// VK_IMAGE_LAYOUT_UNDEFINED				:使い道が未定義なレイアウト
// VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL	:描画結果を保存するレイアウト
// VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL	:シェーダーが画像を読み取るだけなレイアウト
// VK_IMAGE_LAYOUT_PRESENT_SRC_KHR			:画面に画像を表示するレイアウト
// その他レイアウト	:https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkImageLayout.html



// サブパスについて
// 画像データをどう処理するかについて定義するための構造体
// 使用しないアタッチメントについては省略して書いても構わない。
// 以下サブパスに入るアタッチメントの内容
// inputAttachments(入力アタッチメント)					:読み込むデータ(アタッチメント)
// colorAttachments(カラーアタッチメント)				:出力結果を書き込むデータ(アタッチメント)
// resolveAttachments(リゾルブアタッチメント)			:マルチサンプリングされた画像を、通常の画像に変換して保存するデータ(アタッチメント)
// ※リゾルブアタッチメントの数はカラーアタッチメントの数と同じと自動で推測される
// depthStencilAttachment(深度ステンシルアタッチメント)	:深度(奥行き)やステンシル(ピクセル単位の描画制御)情報を保存するデータ(アタッチメント)
// ※深度ステンシルは1つしかないので数を書き込む必要がない
// preserveAttachments(保存アタッチメント)				:次のサブパスで引き続き使うために保存しておくデータ(アタッチメント)を指定

// vk::AttachmentReferenceについて
// attachment	:使用するアタッチメントを指定するインデックス番号(よく分らん)
// layout		:そのアタッチメントがどのようなイメージレイアウトで使用されるかを指定する。

// pipelineBindPoint
// 描画処理に使うか計算処理に使うかを決める
// 
// VK_PIPELINE_BIND_POINT_GRAPHICS	:描画用のパイプラインに結び付ける
// VK_PIPELINE_BIND_POINT_COMPUTE	:計算用のパイプラインに結び付ける(拡張機能が必要)


// サブパス依存関係について
// サブパス間の依存関係を定義するための構造体。
// あるサブパスが他のサブパスの処理が終わるまで待つように指示できる。
// 
// srcSubpass(ソースサブパス)
// dstSubpass(デスティネーションサブパス)について
// ソースサブパスは先に処理されるサブパス
// デスティネーションサブパスはソースサブパスの後に処理される

// StageMask()
// 
// 
// 
// 
// 
// 
// 
// 







































