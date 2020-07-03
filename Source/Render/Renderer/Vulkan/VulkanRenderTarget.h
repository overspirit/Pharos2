#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanRenderTarget : public RenderTarget
		{
		public:
			VulkanRenderTarget(VkDevice device);
			virtual ~VulkanRenderTarget(void);

		protected:
			VkDevice			m_device;
			
			VkRenderPass		m_renderPass;
			VkClearValue		m_clearValues[2];

		protected:
			VkRenderPass CreateRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthStencilFormat = VK_FORMAT_MAX_ENUM);
			VkImageView CreateSurfaceImageViews(VkDevice device, VkFormat format, VkImage image, VkImageAspectFlags flag);

		public:
			VkRenderPass GetCurrRenderPass() { return m_renderPass; }

			virtual VkRenderPassBeginInfo GetRenderPassBeginInfo();

		public:
			virtual bool Init(int32 width, int32 height);

			virtual void SetClear(Color4 color = 0xFF000000, float32 depth = 1.0f, uint32 stencil = 0);

			virtual RenderTexture* GenerateColorAttach(uint32 slot, EPixelFormat fmt);
			virtual void SetColorAttach(uint32 slot, RenderTexture* tex);
			virtual RenderTexture* GenerateDepthAttach(EPixelFormat fmt);
			virtual void SetDepthAttach(RenderTexture* tex);
			virtual RenderTexture* GenerateStencilAttach(EPixelFormat fmt);
			virtual void SetStencilAttach(RenderTexture* tex);
			
			virtual RenderTexture* GetColorAttachTexture(uint32 slot);
			virtual RenderTexture* GetDepthAttachTexture();
			virtual RenderTexture* GetStencilAttachTexture();
			
			virtual EPixelFormat GetColorAttachFormat(uint32 slot);
			virtual EPixelFormat GetDepthAttachFormat();
			virtual EPixelFormat GetStencilAttachFormat();
		};
	}
}
