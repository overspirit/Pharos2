#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanRenderPipeline : public RenderPipeline
		{
		public:
			VulkanRenderPipeline(VkDevice device);
			virtual ~VulkanRenderPipeline();

		private:
			VkDevice	m_device;

			VkPipelineShaderStageCreateInfo*		m_shaderStages;
			
			VkVertexInputBindingDescription      m_vertexBinding;
			VkVertexInputAttributeDescription    m_vertexAttribute[2];
			
			VkPipelineVertexInputStateCreateInfo	m_vertInputState;

			VkPipeline		m_pipeline;

		public:
			VkPipeline GetVulkanPipeline() { return m_pipeline; }

		public:
			virtual bool SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum);

			virtual bool SetProgramShader(RenderProgram* program);

			virtual void SetRenderTarget(VkPipelineLayout pipelineLayout, VulkanRenderTarget* target);

			virtual void SetBlendState(RenderBlendState* state);
			virtual void SetRasterizerState(RenderRasterizerState* state);
			virtual void SetDepthStencilState(RenderDepthStencilState* state);
		};
	}
}
