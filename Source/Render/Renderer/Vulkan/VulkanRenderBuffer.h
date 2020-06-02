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

			VkBuffer			m_buffer;
			VkDeviceMemory		m_memory;

			uint32				m_reqSize;

		public:
			VkBuffer GetBuffer() { return m_buffer; }

		public:
			virtual bool Allocate(uint32 bufSize, MemoryBuffer* buf = nullptr);

			virtual void CopyData(const MemoryBuffer& data, uint32 offset = 0);
			virtual void CopyData(const void* data, uint32 len, uint32 offset = 0);

			virtual void Apply(VkCommandBuffer cmdBuf, uint32 slot);
		};
	}
}
