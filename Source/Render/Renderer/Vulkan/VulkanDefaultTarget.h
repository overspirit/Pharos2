#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanDefaultTarget : public VulkanRenderTarget
		{
		public:
			VulkanDefaultTarget(VkDevice device, int32 width, int32 height);
			virtual ~VulkanDefaultTarget(void);

		private:			
			VkSwapchainKHR		m_swapchain;	

			VkSemaphore			m_semaphore;		

			vector<VkImage>			m_colorImages;
			vector<VkImageView>		m_colorImageViews;

			vector<VkFramebuffer>	m_frameBufList;			
			uint32					m_currFrameIndex;

			VkFence				m_swapchainFence;

		private:
			bool CreateColorAttachment(VkFormat colorFormat);
			void CreateRenderPass(VkFormat colorFormat, VkFormat depthStencilFormat);

		public:
			bool InitDefaultTarget();

			void PresentQueue(VkQueue queue);

			VkFence GetCurrentFence() { return m_swapchainFence; }
			VkSemaphore GetSemaphore() { return m_semaphore; }

			virtual VkRenderPassBeginInfo MakeRenderPassBeginInfo();

		public:
			virtual void SetColorAttach(uint32 slot, RenderTexture* tex);		
			virtual RenderTexture* GetColorAttachTexture(uint32 slot);

			virtual void SetDepthStencilAttach(RenderTexture* tex);
			virtual RenderTexture* GetDepthStencilAttachTexture();
			
		};
	}
}
