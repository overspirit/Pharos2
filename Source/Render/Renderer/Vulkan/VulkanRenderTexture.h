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
			VkImage						m_image;
            VkDeviceMemory				m_deviceMemory;
            uint32_t 					m_mipLevels;

			bool	m_isUpdated;

		private:
			virtual VkBuffer CreateStagingBuffer(const void* imageData, uint32 imageSize);
			virtual bool CopyStagingImage(VkBuffer staging_buffer, const vector<VkBufferImageCopy>& buffer_copy_regions);

        public:
			virtual bool Create(int32 width, int32 height, EPixelFormat fmt);
			virtual bool LoadFromFile(const char8* szPath);
			virtual bool LoadFromImage(const Image* pImage);

			VkDescriptorImageInfo& GetVulkanImageInfo() { return m_imageInfo; }
			bool IsUpdated() { return m_isUpdated; }
			void SetUpdated(bool updated) { m_isUpdated = updated;}

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
