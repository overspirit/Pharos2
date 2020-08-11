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

			//slot 可能不连续， 这个 vector 的 key 为 slot
			// value 为 m_layoutBindings 和 m_writeDescSets 的序号
			// 这个 vector 初始化为长度为 16 的数组
			vector<uint8>		m_slotList;		
			
			VkDescriptorSetLayout	m_descSetLayout;
			VkDescriptorSet			m_descSet;

		public:
			VkDescriptorSet GetVulkanDescriptorSet() { return m_descSet; }
			VkDescriptorSetLayout GetVulkanDescriptorSetLayout() { return m_descSetLayout; }

		public:
            virtual bool SetVertexUniformBuffer(uint32 slot, RenderBuffer* buffer);
            virtual bool SetFragmentUniformBuffer(uint32 slot, RenderBuffer* buffer);

            virtual bool SetVertexTexture(uint32 slot, RenderTexture* tex);
            virtual bool SetFragmentTexture(uint32 slot, RenderTexture* tex);

			virtual void UpdateSet();
		};
	}
}
