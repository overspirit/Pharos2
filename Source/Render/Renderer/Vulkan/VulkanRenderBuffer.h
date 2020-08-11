#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanRenderBuffer : public RenderBuffer
		{
		public:
			VulkanRenderBuffer(BufferType type, VkDevice device);
			virtual ~VulkanRenderBuffer();

		protected:
			VkDevice			m_device;

			VkDescriptorBufferInfo		m_bufferInfo;

			VkDeviceMemory		m_memory;
			//uint8*				m_mapPtr;

			uint32				m_reqSize;

		public:
			VkDescriptorBufferInfo& GetVulkanBufferInfo() { return m_bufferInfo; }

		public:
			virtual bool Allocate(uint32 bufSize, MemoryBuffer* buf = nullptr);

			virtual void CopyData(const MemoryBuffer& dataBuf, uint32 offset = 0);
			virtual void CopyData(const void* data, uint32 len, uint32 offset = 0);
		};
	}
}
