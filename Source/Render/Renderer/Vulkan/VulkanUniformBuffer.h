#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanUniformBuffer : public RenderBuffer
		{
		public:
			VulkanUniformBuffer(BufferType type, VkDevice device);
			virtual ~VulkanUniformBuffer();

		private:
			VkDevice	m_device;

		public:

		public:
			virtual bool Allocate(uint32 bufSize, MemoryBuffer* buf = nullptr);

			virtual void CopyData(const MemoryBuffer& data, uint32 offset = 0);
			virtual void CopyData(const void* data, uint32 len, uint32 offset = 0);
		};
	}
}
