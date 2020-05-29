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

		private:
			VkDevice			m_device;
			VkSwapchainKHR		m_swapchain;
			
			VkRenderPass		m_renderPass;
			VkClearValue		m_clearValues[2];

			vector<VkImage>		m_swapchainImages;
			vector<VkImageView>	m_swapchainImageViews;

			vector<VkFramebuffer>	m_frameBufList;
			VkSemaphore			m_semaphore;
			uint32				m_currFrameIndex;

		private:
			VkRenderPass CreateRenderPass(VkDevice device, VkFormat format);
			std::vector<VkImageView> CreateSurfaceImageViews(VkDevice device, VkFormat surfaceFormat, const vector<VkImage> surfaceImages);

		public:
			bool CreateDefaultFrameBuffer(VkSwapchainKHR swapchain, VkSemaphore semaphore, int32 width, int32 height, VkFormat format);
			VkRenderPass GetCurrRenderPass() { return m_renderPass; }
			VkRenderPassBeginInfo GetRenderPassBeginInfo();
			void PresentQueue(VkQueue queue);

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
