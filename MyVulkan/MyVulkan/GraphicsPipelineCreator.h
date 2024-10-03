#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "Utilities.h"
#include "ShaderUtility.h"
#include "ViewportGenerator.h"
#include "PipelineShaderCreator.h"
#include "PipelineDescriptions.h"
#include "Scene.h"

class PipelineGenerator : CGeneratorBase
{
public:
	PipelineGenerator();
	~PipelineGenerator();

	void Create(
		vk::Device logicalDevice, 
		vk::RenderPass renderPass, 
		vk::Extent2D extent, 
		std::vector<vk::PipelineShaderStageCreateInfo>shaderStageInfos);
	void Destroy(vk::Device logicalDevice);

	vk::Pipeline		GetPipeline();
	vk::PipelineLayout	GetPipelineLayout();

	int createTextureDescriptor(vk::Device logicalDevice, VkImageView textureImage);

private:
	vk::Device						m_LogicalDevice;

	vk::Pipeline					m_Pipeline;

	vk::PipelineLayout				m_PipelineLayout;

	//PipelineShaderCreator			m_ShaderLoader;

	// デスクリプタ
	DescriptorHandler				m_TextureDescriptor;


	vk::GraphicsPipelineCreateInfo						m_PipelineInfo;
	vk::PipelineLayoutCreateInfo						m_PipelineLayoutInfo;
	std::vector<vk::PipelineShaderStageCreateInfo>		m_ShaderStageInfos;
	vk::PipelineMultisampleStateCreateInfo				m_MultisamplingInfo;
	vk::PipelineVertexInputStateCreateInfo				m_VertexInputCreateInfo;
	vk::PipelineInputAssemblyStateCreateInfo			m_InputAssemblyInfo;
	vk::PipelineViewportStateCreateInfo					m_ViewportInfo;
	vk::PipelineRasterizationStateCreateInfo			m_RasterizerInfo;
	std::vector<vk::PipelineColorBlendAttachmentState>	m_ColorBlendAttachment;
	vk::PipelineColorBlendStateCreateInfo				m_ColorBlendInfo;
	vk::PipelineDepthStencilStateCreateInfo				m_DepthStencilInfo;

	vk::Pipeline		CreateGraphicsPipeline(vk::Device logicalDevice, vk::RenderPass renderPass, vk::Extent2D extent, std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfo);
	vk::PipelineLayout	CreatePipelineLayout(vk::Device logicalDevice, std::vector<vk::DescriptorSetLayout> descriptorSetLayouts);


	vk::PipelineViewportStateCreateInfo			CreateViewportStateInfo(vk::Extent2D extent);
	vk::PipelineRasterizationStateCreateInfo	CreateRasterizerStateInfo();
	vk::PipelineMultisampleStateCreateInfo		CreateMultisampleStateInfo(vk::SampleCountFlagBits sampleValue);
	vk::PipelineColorBlendStateCreateInfo		CreateColorBlendingStateInfo();
	vk::PipelineDepthStencilStateCreateInfo		CreateDepthStencilStateInfo(bool depth, bool stencil);
	
/*	std::vector<vk::PipelineShaderStageCreateInfo>		GetShaderStageInfo(vk::Device logicalDevice);
	vk::PipelineVertexInputStateCreateInfo				GetVertexInputStateInfo();
	vk::PipelineInputAssemblyStateCreateInfo			GetInputAssemblyStateInfo();
	vk::PipelineRasterizationStateCreateInfo			GetRasterizationStateInfo();

	vk::PipelineMultisampleStateCreateInfo				GetMultisampleStateInfo();
	std::vector<vk::PipelineColorBlendAttachmentState>	GetColorBlendAttachmentState();
	vk::PipelineColorBlendStateCreateInfo				GetColorBlendStateInfo(std::vector<vk::PipelineColorBlendAttachmentState>* colorBlendAttachment)*/;

};

// グラフィックスパイプラインについて
// 頂点情報から画像の色情報に変換するまでの処理の流れを決めるオブジェクト。
// グラフィックスパイプラインの作成にはレンダーパスが必須。
// 複数のサブパスを組み合わせて描画を組み合わせることができる。
// 以下の流れで情報を変換いく。
// 
// 1.頂点情報の入力(入力アセンブラ)
// 頂点のデータを読み込む。
// インデックスバッファーに格納する。
// 
// 2.頂点シェーダー
// 入力した頂点情報(インデックスバッファー)から画面上の表示位置を計算する。
// 3Dオブジェクトの移動や回転、拡大縮小などの計算はここで行う。
// このシェーダーは必ず用意する必要がある。
// 
// 3.テッセレーション・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
// ジオメトリ(物体の形状)を細分化して、オブジェクトの表面を滑らかにしたり、肌感を作る。
// テッセレーション制御シェーダ: テッセレーションの細かさを決める。
// テッセレーションエンジン: パッチを細かく分割する。(この処理はハードウェア(GPU)内で自動的にやってくれる。)
// テッセレーション評価シェーダ(ドメインシェーダ): 新しい頂点の位置を計算する。
// この処理はオプションなので使用しないこともできる。
// 
// 4.ジオメトリーシェーダー
// プリミティブ(三角形などの計算に使いやすい基本図形)に対して追加で加工を加える。
// 頂点数を増減させて、新しい形状にしたりできる。
// 重たい処理なのであまり多用されない。
// この処理はオプションなので使用しないこともできる。
// 
// 5.ラスター化
// 3D空間で定義されたジオメトリ(頂点やポリゴンなど)を
// 2Dのフラグメント(ピクセルに対応するデータ)に変換する。
// この処理はハードウェア(GPU)内で自動的にやってくれる。
// 
// 6.フラグメントシェーダー(ピクセルシェーダー)
// 点(ピクセル)それぞれに付ける色を計算するシェーダー。
// 基本的に面の方向や材質、視点や光の位置等の情報を考慮して計算する。
// このシェーダーは必ず用意する必要がある。
// 
// 7.カラー出力 (Color Output)
// 上のフラグメントシェーダーで出した結果を
// フレームバッファのデータに実際に書き込む。
// この処理はハードウェア(GPU)内で自動的にやってくれる。
// 


// シェーダーとは
// GPUで動かす用のプログラム。
// 元々は陰影の計算に使うための物だった為、このような名前になっている。
// VulkanではHLSLやはGLSL等のシェーダー用の言語で書いたプログラムを
// SPIR-Vという中間言語(バイナリ形式のコード)にコンパイルして使用する。
// グラフィックスパイプラインでは頂点・ピクセルシェーダーの2つは
// 最低限、自分で用意しなければならない。
// デフォルトのシェーダーとかは用意されていない。


// SPIR-Vとは
//「Standard Portable Intermediate Representation - Version V」
// (標準的で移植可能な中間表現)の略。
// Vulkanではシェーダーのコードをぞのまま使用することは出来ないため、
// コンパイルする必要がある。
// コンパイルしたものがSPIR-V。
// Vulkan SDKに付属のglslangValidator.exeからコンパイルできる。
// SPIRV-Reflect : https://github.com/KhronosGroup/SPIRV-Reflect
// SPIRV-Cross	 : https://github.com/KhronosGroup/SPIRV-Cross
// 

// パイプラインレイアウトとは
// 頂点に含まれる情報(座標や色など)やテクスチャ等を
// シェーダーが読めるように定義するためのオブジェクト。
// 






