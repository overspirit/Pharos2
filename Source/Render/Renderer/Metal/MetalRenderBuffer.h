#pragma once

namespace Pharos
{
	namespace Render
	{
		const static uint32 VERTEX_BUFFER_BEGIN_SLOT = 8;
	
		class MetalRenderBuffer : public RenderBuffer
		{
		public:
			MetalRenderBuffer(BufferType type, id<MTLDevice> device);
			virtual ~MetalRenderBuffer();

		private:
			id<MTLDevice>   m_device;
			id<MTLBuffer>   m_buffer;
			
		public:
			id<MTLBuffer> GetMetalBuffer() { return m_buffer; }
			
		public:
			virtual bool Allocate(uint32 bufSize, MemoryBuffer* buf = nullptr);
			
			virtual void CopyData(const MemoryBuffer& data, uint32 offset = 0);
			virtual void CopyData(const void* data, uint32 len, uint32 offset = 0);

			virtual void ApplyVertexUniformBuffer(uint32 slot, id<MTLRenderCommandEncoder> encoder);
			virtual void ApplyFragmentUniformBuffer(uint32 slot, id<MTLRenderCommandEncoder> encoder);
		};
	}
}
