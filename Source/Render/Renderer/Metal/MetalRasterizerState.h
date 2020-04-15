#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRasterizerState : public RenderRasterizerState
		{
		public:
			MetalRasterizerState();
			virtual ~MetalRasterizerState(void);

		private:

		public:
			virtual bool CreateState(const RasterizerStateDesc& desc);

			virtual RenderRasterizerState* Clone();

			virtual void ApplyDevice();
		};
	}
}
