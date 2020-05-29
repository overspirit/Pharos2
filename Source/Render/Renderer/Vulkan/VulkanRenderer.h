#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanRenderer : public Renderer
		{
		public:
			VulkanRenderer(void);
			virtual ~VulkanRenderer(void);

		private:
			VkDevice			m_device;

			VkCommandPool		m_cmdPool;
			VkCommandBuffer		m_cmdBuf;

			VkQueue				m_queue;
			VkSemaphore			m_semaphore;

			VulkanRenderTarget*		m_defaultTarget;

		private:
			VkCommandPool CreateCommandPool(VkDevice device, uint32 queueFamilyIndex);
			VkCommandBuffer CreateCommandBuffer(VkDevice device, VkCommandPool cmdPool);
			VkSemaphore CreateSemaphore(VkDevice device);
			VkFence CreateDrawFence(VkDevice device);

		public:
			virtual bool Init();
			virtual bool Create(const DeviceConfig& cfg);
			virtual void Destroy();
			
			virtual void Commit();
			
			virtual RenderTarget* GetDefaultRenderTarget() { return m_defaultTarget; }
			virtual const char8* GetAdapterName() const { return ""; }
			virtual uint32 GetAdapterMemorySize() const { return 0; }
            
			virtual RenderBuffer* GenerateRenderBuffer(BufferType type);
			virtual RenderTexture* CreateTexture(int32 width, int32 height, EPixelFormat fmt);
			virtual RenderTexture* LoadTexture(const char8* szPath);
			virtual RenderTexture* LoadTexture(const Image* image);
            
            virtual RenderProgram* GenerateRenderProgram();
			virtual RenderTarget* CreateRenderTarget(int32 width, int32 height);
			virtual RenderSamplerState* CreateSampleState(const SamplerStateDesc& desc);
			virtual RenderBlendState* CreateBlendState(const BlendStateDesc& desc);
			virtual RenderRasterizerState* CreateRasterizerState(const RasterizerStateDesc& desc);
			virtual RenderDepthStencilState* CreateDepthStencilState(const DepthStencilStateDesc& desc);
            virtual RenderPipeline* GenerateRenderPipeline();
            
            virtual RenderCommand* GenerateRenderCommand(RenderTarget* renderTarget);
		};
	}
}
