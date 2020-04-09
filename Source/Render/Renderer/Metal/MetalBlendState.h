#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalBlendState : public RenderBlendState
		{
		public:
			MetalBlendState();
			virtual ~MetalBlendState(void);

		private:
			
		public:
			virtual bool CreateState(const BlendStateDesc& desc);

			virtual RenderBlendState* Clone();

			virtual void ApplyDevice();
		};
	}
}
