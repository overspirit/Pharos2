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

			vector<VkDescriptorSetLayout>	m_setLayouts;
			VkPipelineLayout				m_pipelineLayout;

		public:
			void CreatePipelineLayout();			

		public:			
			virtual void SetDebugLabel(const char8* label);
			
			virtual void BeginCommand();
			
			virtual void SetVertexBuffer(uint32 slot, RenderBuffer* buffer);
			virtual void SetFragmentBuffer(uint32 slot, RenderBuffer* buffer);
			virtual void SetFragmentTexture(uint32 slot, RenderTexture* tex);

			virtual void SetPipeline(RenderPipeline* pipeline);

			virtual void DrawImmediate(DrawType type, uint32 start, uint32 count);
            
            virtual void EndCommand();
		};
	}
}
