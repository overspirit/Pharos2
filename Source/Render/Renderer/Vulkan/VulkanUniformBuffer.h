#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanUniformBuffer : public VulkanRenderBuffer
		{
		public:
			VulkanUniformBuffer(BufferType type, VkDevice device);
			virtual ~VulkanUniformBuffer();

		private:
			VkDescriptorSetLayout	m_descSetLayout;
			VkDescriptorSet			m_descSet;

		public:
			VkDescriptorSetLayout GetDescriptorSetLayout() { return m_descSetLayout; }
			VkDescriptorSet GetDescriptorSet() { return m_descSet; }

		public:
			virtual void Apply(VkDescriptorPool descPool, uint32 slot);
		};
	}
}
