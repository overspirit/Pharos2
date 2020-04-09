#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalDepthStencilState : public RenderDepthStencilState
		{
		public:
			MetalDepthStencilState();
			virtual ~MetalDepthStencilState(void);

		private:

		public:
			virtual bool CreateState(const DepthStencilStateDesc& desc);
			
			virtual RenderDepthStencilState* Clone();

			virtual void ApplyDevice();
		};
	}
}
