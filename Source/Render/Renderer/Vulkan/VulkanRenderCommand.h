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

		private:
			void BuildPipelineLayout(DrawType type);

		public:
			virtual void SetDebugLabel(const char8* label);

			virtual void BeginCommand();

			virtual void SetVertexBuffer(RenderBuffer* buffer);
			virtual void SetIndexBuffer(RenderBuffer* indexBuffer, IndexElementType indexType);	

			virtual void SetViewport(const Rect2Di& viewRect, float32 minDepth, float32 maxDepth);
			virtual void SetScissorRect(const Rect2Di& scissorRect);

			virtual void SetPipeline(RenderResourceSet* resSet, RenderPipeline* pipeline);

			virtual void DrawPrimitives(DrawType type, uint32 start, uint32 count);
			virtual void DrawIndexedPrimitives(DrawType type, uint32 start, uint32 count);

            virtual void EndCommand();
		};
	}
}
