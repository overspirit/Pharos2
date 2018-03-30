#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11RenderPass : public RenderPass
		{
		public:
			D3D11RenderPass();
			virtual ~D3D11RenderPass();

		private:
			D3D11Renderer*			m_renderer;

			D3D11ShaderProgram*		m_shader;

 			D3D11BlendState*			m_blendState;
 			D3D11DepthStencilState*		m_depthState;
 			D3D11RasterizerState*		m_rasterizerState;

			D3D11FrameBuffer*		m_frameBuf;

			string					m_strError;

		public:
			virtual bool CreateShaderProgram(const char8* vertEnter, const char8* pixelEnter, const char8* shaderText);

			virtual D3D11RenderPass* Clone();

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