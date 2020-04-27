#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRenderBuffer : public RenderBuffer
		{
		public:
			MetalRenderBuffer(id<MTLDevice> device);
			virtual ~MetalRenderBuffer();

		private:
			id<MTLDevice>   m_device;
			id<MTLBuffer>   m_buffer;

		public:
			id<MTLBuffer> GetMetalBuffer() { return m_buffer; }

		public:
			virtual bool CreateBuffer(uint32 bufSize, MemoryBuffer* buf = nullptr);

		public:
			virtual void CopyData(const MemoryBuffer& data, uint32 offset = 0);
			virtual void CopyData(const void* data, uint32 len, uint32 offset = 0);

			virtual void ApplyDevice(uint32 slot);
		};
	}
}
