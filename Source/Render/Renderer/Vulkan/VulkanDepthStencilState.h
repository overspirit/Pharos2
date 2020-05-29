#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanDepthStencilState : public RenderDepthStencilState
		{
		public:
			VulkanDepthStencilState();
			virtual ~VulkanDepthStencilState(void);

		private:
			VkPipelineDepthStencilStateCreateInfo	m_depthStencilState;

		public:
			VkPipelineDepthStencilStateCreateInfo GetStateCreateInfo() { return m_depthStencilState; }

		public:
			virtual bool CreateState(const DepthStencilStateDesc& desc);

			virtual RenderDepthStencilState* Clone();

			virtual void ApplyDevice();
		};
	}
}
