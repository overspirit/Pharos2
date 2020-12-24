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
			
            id<MTLDepthStencilState>       m_depthStencilState;            
            
			id<MTLRenderPipelineState>		m_pipelineState;
			MTLRenderPipelineDescriptor*	m_pipelineStateDesc;
						
			DrawType		m_drawType;
		
		private:
			MTLVertexFormat GetMetalFormat(VertElementType elementType, uint32 elementNum);
			uint32 GetVertElementSize(VertElementType elementType, uint32 elementNum);

		public:
			virtual bool SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum);
            virtual void SetTargetFormat(EPixelFormat colorFmt[], EPixelFormat depthFmt, EPixelFormat stencilFmt);
			virtual bool SetProgramShader(RenderProgram* program);

            virtual void SetBlendState(const BlendStateDesc& state);
            virtual void SetRasterizerState(const RasterizerStateDesc& state);
            virtual void SetDepthStencilState(const DepthStencilStateDesc& state);
			
			virtual void SetDrawType(DrawType drawType) { m_drawType = drawType; }
			virtual DrawType GetDrawType() { return m_drawType; }
						
			virtual void ApplyToEncoder(id<MTLRenderCommandEncoder> encoder);
		};
	}
}
