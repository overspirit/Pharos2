#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalSamplerState : public RenderSamplerState
		{
		public:
			MetalSamplerState();
			virtual ~MetalSamplerState(void);

		private:
            
		public:
			virtual bool CreateState(const SamplerStateDesc& desc);

			virtual RenderSamplerState* Clone();

			virtual void ApplyDevice(uint32 slot);
		};
	}
}
