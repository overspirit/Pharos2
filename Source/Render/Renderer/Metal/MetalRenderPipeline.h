#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRenderPipeline : public RenderPipeline
		{
		public:
			MetalRenderPipeline(id<MTLDevice> device);
			virtual ~MetalRenderPipeline();

		private:
			id<MTLDevice>   m_device;

			id<MTLRenderPipelineState>  m_pipelineState;

		private:
			MTLVertexFormat GetMetalFormat(VertElementType elementType, uint32 elementNum);
			uint32 GetVertElementSize(VertElementType elementType, uint32 elementNum);

		public:
			id<MTLRenderPipelineState>  GetMetalPipeline() { return m_pipelineState; }

		public:
			virtual bool SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum);

			virtual bool SetVertexShader(RenderProgram* program, const char8* enter);
			virtual bool SetPixelShader(RenderProgram* program, const char8* enter);

			virtual void SetRenderTarget(RenderTarget* target);

			virtual void SetBlendState(RenderBlendState* state);
			virtual void SetRasterizerState(RenderRasterizerState* state);
			virtual void SetDepthStencilState(RenderDepthStencilState* state);
		};
	}
}
