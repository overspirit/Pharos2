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

		private:
			VkDevice	m_device;
			VkBuffer	m_buffer;
			VkDeviceMemory		m_memory;

		public:
			VkBuffer GetBuffer() { return m_buffer; }

		public:
			virtual bool Allocate(uint32 bufSize, MemoryBuffer* buf = nullptr);

			virtual void CopyData(const MemoryBuffer& data, uint32 offset = 0);
			virtual void CopyData(const void* data, uint32 len, uint32 offset = 0);

			virtual void ApplyDevice(uint32 slot);
		};
	}
}
