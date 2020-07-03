#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanDesciptorSet : public RenderResourceSet
		{
		public:
			VulkanDesciptorSet(VkDevice device, VkDescriptorPool descPool);
			virtual ~VulkanDesciptorSet();

		private:
			VkDevice				m_device;
			VkDescriptorPool		m_descPool;

			vector<VkDescriptorSetLayoutBinding>	m_layoutBindings;
			vector<VkWriteDescriptorSet>			m_writeDescSets;
			
			VkDescriptorSetLayout	m_descSetLayout;
			VkDescriptorSet			m_descSet;

		public:
			VkDescriptorSet GetVulkanDescriptorSet() { return m_descSet; }
			VkDescriptorSetLayout GetVulkanDescriptorSetLayout() { return m_descSetLayout; }

		public:
            virtual void SetVertexUniformBuffer(uint32 slot, RenderBuffer* buffer);
            virtual void SetFragmentUniformBuffer(uint32 slot, RenderBuffer* buffer);

            virtual void SetVertexTexture(uint32 slot, RenderTexture* tex);
            virtual void SetFragmentTexture(uint32 slot, RenderTexture* tex);

			virtual bool ApplyResourceSet();
		};
	}
}
