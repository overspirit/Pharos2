#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanDefaultTarget : public VulkanRenderTarget
		{
		public:
			VulkanDefaultTarget(VkDevice device, VkSemaphore semaphore);
			virtual ~VulkanDefaultTarget(void);

		private:
			VkSemaphore			m_semaphore;
			VkSwapchainKHR		m_swapchain;			

			vector<VkImage>		m_swapchainImages;
			vector<VkImageView>	m_swapchainImageViews;
			vector<VkFence>		m_swapchainFence;

			vector<VkFramebuffer>	m_frameBufList;			
			uint32					m_currFrameIndex;

			Color4f			m_clearColor;
			float32			m_clearDepth;
			float32 		m_clearStencil;

		private:
			VkImage CreateDepthImage(int width, int height, VkFormat depth_format);
			VkFence CreateDrawFence(VkDevice device);

		public:
			bool CreateDefaultTarget(VkSwapchainKHR swapchain, int32 width, int32 height, VkFormat colorFormat, VkFormat depthFormat);
			void PresentQueue(VkQueue queue);
			VkFence GetCurrentFence() { return m_swapchainFence[0]; }

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
