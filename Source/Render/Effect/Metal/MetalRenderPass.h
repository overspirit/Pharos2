#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRenderPass : public RenderPass
		{
		public:
			MetalRenderPass();
			virtual ~MetalRenderPass();

		private:
			MetalRenderer*			m_renderer;

			MetalShaderProgram*		m_shader;

 			MetalBlendState*			m_blendState;
 			MetalDepthStencilState*		m_depthState;
 			MetalRasterizerState*		m_rasterizerState;

			MetalFrameBuffer*		m_frameBuf;

			string					m_strError;

		public:
			virtual bool CreateShaderProgram(const char8* vertEnter, const char8* pixelEnter, const char8* shaderText);

			virtual MetalRenderPass* Clone();

			virtual void BindShaderProgram(RenderProgram* shader);
			virtual void BindFrameBuffer(RenderFrameBuffer* frameBuf);
			virtual void BindRasterizerState(RenderRasterizerState* state);
			virtual void BindDepthStencilState(RenderDepthStencilState* state);
			virtual void BindBlendState(RenderBlendState* state);

			virtual RenderProgram* GetShaderProgram() { return m_shader; }

			virtual void ApplyToDevice();
		};
	}
}
