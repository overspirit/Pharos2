#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanRenderTexture : public RenderTexture
		{
		public:
			VulkanRenderTexture(VkDevice device, VkCommandPool cmdPool, VkQueue queue);
			virtual ~VulkanRenderTexture();

		private:
			VkDevice		m_device;
			VkCommandPool	m_cmdPool;
			VkQueue			m_queue;			
        
			VkDescriptorImageInfo		m_imageInfo;

			uint32_t 						m_mipLevels;
			vector<VkBufferImageCopy>		m_copyRegions;

			VkImage						m_image;
			VkImageView					m_imageView;
			VkSampler					m_sampler;
			
			VkBuffer					m_stagingBuffer;
			VkDeviceMemory				m_stagingMemory;
			bool						m_isNeedUpdate;

			VkDeviceMemory				m_deviceMemory;			

		private:
			bool CreateVulkanStagingBuffer(uint32 imageSize);
			bool CreateVulkanSampler();
			bool CreateVulkanImage(); 		

		public:
			bool FlushVulkanImage();

			VkDescriptorImageInfo& GetVulkanImageInfo() { return m_imageInfo; }
			
		public:
			virtual bool Create(int32 width, int32 height, EPixelFormat fmt);
			virtual bool LoadFromFile(const char8* szPath);
			virtual bool LoadFromImage(const Image* pImage);	

		public:
			virtual bool CopyFromData(const void* pImageData, uint32 nDataSize);
			virtual bool CopyRectFromData(const void* pData, uint32 nDataSize, const Rect2Di& rt);
			virtual bool CopyFromTexture(RenderTexture* srcTex);
			virtual bool CopyRectFromTexture(RenderTexture* srcTex, const Rect2Di& srcRect, const Rect2Di& destRect);

			virtual void SetSampleState(RenderSamplerState* state);

			virtual uint32 GetWidth() const { return m_width; }
			virtual uint32 GetHeight() const { return m_height; }
			virtual EPixelFormat GetFormat() const { return m_fmt; }
		};
	}
}
