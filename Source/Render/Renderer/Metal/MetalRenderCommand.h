#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRenderCommand : public RenderCommand
		{
		public:
			MetalRenderCommand(id<MTLCommandQueue> commandQueue, MTKView* view);
			virtual ~MetalRenderCommand();

		private:
			id<MTLCommandQueue>             m_commandQueue;
			MTKView*                        m_view;
			MTLRenderPassDescriptor*        m_renderPassDescriptor;

			id <MTLCommandBuffer>           m_commandBuffer;
			id <MTLRenderCommandEncoder>    m_renderEncoder;

			//id<MTLRenderPipelineState>  m_pipeline;
		   // id<MTLBuffer>               m_vertBuf;

			//id<MTLTexture>              m_texture;

		private:
			void BuildEncoder();

		public:
			virtual bool Init();

			virtual void SetVertexBuffer(uint32 slot, RenderBuffer* buffer);
			virtual void SetFragmentBuffer(uint32 slot, RenderBuffer* buffer);
			virtual void SetFragmentTexture(uint32 slot, RenderTexture* tex);

			virtual void SetPipeline(RenderPipeline* pipeline);

			virtual void DrawImmediate(DrawType type, uint32 start, uint32 count);

			virtual bool Present();
		};
	}
}
