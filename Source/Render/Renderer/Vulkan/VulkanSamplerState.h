#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanSamplerState : public RenderSamplerState
		{
		public:
			VulkanSamplerState();
			virtual ~VulkanSamplerState(void);

		private:
            
		public:
			virtual bool CreateState(const SamplerStateDesc& desc);

			virtual RenderSamplerState* Clone();

			virtual void ApplyDevice(uint32 slot);
		};
	}
}
