#pragma once

namespace Pharos
{
	namespace Render
	{
		class RenderPass
		{
		public:
			virtual ~RenderPass(){}

		public:
			virtual void BindShaderProgram(RenderProgram* shader) = 0;
			virtual void BindFrameBuffer(RenderFrameBuffer* frameBuf) = 0;
			virtual void BindRasterizerState(RenderRasterizerState* state) = 0;
			virtual void BindDepthStencilState(RenderDepthStencilState* state) = 0;
			virtual void BindBlendState(RenderBlendState* state) = 0;

			virtual RenderProgram* GetShaderProgram() = 0;

			virtual void ApplyToDevice() = 0;
		};
	}
}