#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanRenderTarget : public RenderTarget
		{
		public:
			VulkanRenderTarget(VkDevice device, int32 width, int32 height);
			virtual ~VulkanRenderTarget(void);

		protected:
			VkDevice			m_device;
			
			VkRenderPass		m_renderPass;
			VkFramebuffer		m_frameBuffer;	
			VkClearValue		m_clearValues[2];

			vector<VkAttachmentDescription>		m_attachments;
			vector<VkAttachmentReference>		m_colorReferenceList;
			VkAttachmentReference				m_depthReference;

			vector<VulkanRenderTexture*>		m_colorAttachmentList;
			VulkanRenderTexture*				m_depthAttachment;

			vector<VkImageView>			m_imageViewList;

		public:
			VkRenderPass GetCurrRenderPass() { return m_renderPass; }

			virtual VkRenderPassBeginInfo MakeRenderPassBeginInfo();

		public:
			virtual void SetClear(Color4 color, float32 depth = 1.0f, uint32 stencil = 0);

			virtual void SetColorAttach(uint32 slot, RenderTexture* tex);
			virtual RenderTexture* GetColorAttachTexture(uint32 slot);

			virtual void SetDepthStencilAttach(RenderTexture* tex);
			virtual RenderTexture* GetDepthStencilAttachTexture();			
		};
	}
}
