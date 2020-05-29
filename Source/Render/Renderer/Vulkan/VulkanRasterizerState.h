#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanRasterizerState : public RenderRasterizerState
		{
		public:
			VulkanRasterizerState();
			virtual ~VulkanRasterizerState(void);

		private:
			VkPipelineRasterizationStateCreateInfo		m_rasterState;

		public:
			VkPipelineRasterizationStateCreateInfo GetStateCreateInfo() { return m_rasterState; }

		public:
			virtual bool CreateState(const RasterizerStateDesc& desc);

			virtual RenderRasterizerState* Clone();

			virtual void ApplyDevice();
		};
	}
}
