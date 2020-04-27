#include "PreCompile.h"
#include "Pharos.h"

MetalRenderBuffer::MetalRenderBuffer(id<MTLDevice> device)
{
	m_device = device;
}

MetalRenderBuffer::~MetalRenderBuffer()
{
}

bool MetalRenderBuffer::CreateBuffer(uint32 bufSize, MemoryBuffer* buf)
{
	if (buf == nullptr)
	{
		m_buffer = [m_device newBufferWithLength : bufSize options : MTLResourceStorageModeShared];
	}
	else
	{
		void* dataBuffer = buf->GetPointer();

		// 创建顶点缓存
		m_buffer = [m_device newBufferWithBytes : dataBuffer length : bufSize options : MTLResourceStorageModeShared];
	}

	return true;
}

void MetalRenderBuffer::CopyData(const MemoryBuffer& data, uint32 offset)
{
	if (m_buffer == nullptr) return;

	void* dataBuffer = data.GetPointer();
	uint32 dataSize = data.GetLength();

	memcpy((uint8*)m_buffer.contents + offset, dataBuffer, dataSize);
}

void MetalRenderBuffer::CopyData(const void* data, uint32 len, uint32 offset)
{
	if (m_buffer == nullptr) return;

	memcpy((uint8*)m_buffer.contents + offset, data, len);
}

void MetalRenderBuffer::ApplyDevice(uint32 slot)
{
	//    NSUInteger uniformBufferSize = kAlignedUniformsSize * kMaxBuffersInFlight;
	//
	//    _dynamicUniformBuffer = [_device newBufferWithLength:uniformBufferSize
	//                                                 options:MTLResourceStorageModeShared];
	//
	//    _dynamicUniformBuffer.label = @"UniformBuffer";

}
