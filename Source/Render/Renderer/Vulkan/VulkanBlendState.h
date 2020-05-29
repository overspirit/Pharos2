#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanBlendState : public RenderBlendState
		{
		public:
			VulkanBlendState();
			virtual ~VulkanBlendState(void);

		private:
			VkPipelineColorBlendAttachmentState		m_attaState[1];			
			VkPipelineColorBlendStateCreateInfo		m_blendState;

		public:
			VkPipelineColorBlendStateCreateInfo GetStateCreateInfo() { return m_blendState; }			

		public:
			virtual bool CreateState(const BlendStateDesc& desc);

			virtual RenderBlendState* Clone();

			virtual void ApplyDevice();
		};
	}
}
