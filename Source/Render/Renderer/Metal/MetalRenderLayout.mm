#include "PreCompile.h"
#include "Pharos.h"

MetalRenderLayout::MetalRenderLayout()
{
}

MetalRenderLayout::~MetalRenderLayout()
{
}

bool MetalRenderLayout::CreateVertexBuffer(uint32 sizeInBytes, MemoryBuffer* data)
{
	return true;
}

bool MetalRenderLayout::CreateIndexBuffer(uint32 indexNum, MemoryBuffer* data)
{
	return true;
}

void MetalRenderLayout::CopyVertexBuffer(MemoryBuffer* data, uint32 dataSize, uint32 nOffsetInBytes, bool bNoOverwrite)
{
}

void MetalRenderLayout::CopyIndexBuffer(MemoryBuffer* data, uint32 dataSize, uint32 nOffsetInBytes, bool bNoOverwrite)
{
}

bool MetalRenderLayout::SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum)
{
	return true;
}

void MetalRenderLayout::ApplyDevice()
{
}
