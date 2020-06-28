#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanRenderCommand : public RenderCommand
		{
		public:
			VulkanRenderCommand(VkDevice device, VkCommandBuffer cmdBuf, VulkanRenderTarget* renderTarget);
			virtual ~VulkanRenderCommand();

		private:
			VkDevice				m_device;

			VkCommandBuffer			m_cmdBuf;

			VulkanRenderTarget*		m_renderTarget;	

			VulkanRenderPipeline*	m_currentPipeline;
			// VulkanUniformBuffer*	m_currentBuffer;
			// VulkanRenderTexture*	m_currentTexture;

			vector<VkDescriptorSetLayoutBinding>	m_layoutBindings;
        	vector<VkWriteDescriptorSet>			m_writeDescSets;

			VkDescriptorPool		m_descPool;
			VkDescriptorSetLayout	m_descSetLayout;
			VkDescriptorSet			m_descSet;

			VkPipelineLayout		m_pipelineLayout;

			VkViewport				m_viewport;
    		VkRect2D				m_scissor;

		public:		

		public:			
			virtual void SetDebugLabel(const char8* label);
			
			virtual void BeginCommand();
			
			virtual void SetVertexBuffer(uint32 slot, RenderBuffer* buffer);
			virtual void SetFragmentBuffer(uint32 slot, RenderBuffer* buffer);
			virtual void SetFragmentTexture(uint32 slot, RenderTexture* tex);
			virtual void SetIndexBuffer(RenderBuffer* indexBuffer, IndexElementType indexType);	

			virtual void SetViewport(const Rect2Di& viewRect, float32 minDepth, float32 maxDepth);
            virtual void SetScissorRect(const Rect2Di& scissorRect);

			virtual void SetPipeline(RenderPipeline* pipeline);

			virtual void DrawPrimitives(DrawType type, uint32 start, uint32 count);
			virtual void DrawIndexedPrimitives(DrawType type, uint32 indexCount, uint32 indexOffset);

            virtual void EndCommand();
		};
	}
}
