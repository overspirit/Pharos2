#include "PreCompile.h"
#include "Pharos.h"

MetalConstantBuffer::MetalConstantBuffer()
{
}

MetalConstantBuffer::~MetalConstantBuffer()
{
}

void MetalConstantBuffer::CopyData(const MemoryBuffer& data, uint32 offset)
{
}

void MetalConstantBuffer::CopyData(const void* data, uint32 len, uint32 offset)
{
}

bool MetalConstantBuffer::CreateBuffer(uint32 bufSize)
{
    return true;
}

void MetalConstantBuffer::ApplyDevice(uint32 slot)
{
}
