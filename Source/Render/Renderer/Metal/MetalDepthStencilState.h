#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalDepthStencilState : public RenderDepthStencilState
		{
		public:
			MetalDepthStencilState(id<MTLDevice> device);
			virtual ~MetalDepthStencilState(void);

		private:
			id<MTLDevice>   m_device;
			
			id <MTLDepthStencilState>		m_depthState;

		public:
			virtual bool CreateState(const DepthStencilStateDesc& desc);

			virtual RenderDepthStencilState* Clone();

			virtual void ApplyDevice(id <MTLRenderCommandEncoder> renderEncoder);
		};
	}
}
