#include "PreCompile.h"
#include "Pharos.h"

MetalTexture::MetalTexture()
{
}

MetalTexture::~MetalTexture()
{
}

bool MetalTexture::LoadFromFile(const char8* szPath)
{
    return true;
}

bool MetalTexture::LoadFromImage(const Image* image)
{
	return true;
}

bool MetalTexture::Create(int32 width, int32 height, EPixelFormat fmt)
{
	return true;
}

bool MetalTexture::CreateDepthTexture(int32 width, int32 height)
{

	return true;
}

bool MetalTexture::CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt)
{
	return true;
}


bool MetalTexture::CopyFromData(const void* pImageData, uint32 nDataSize)
{
	return true;
}

bool MetalTexture::CopyRectFromData(const void* pData, uint32 nDataSize, const Rect2Di& rt)
{
	return true;
}

bool MetalTexture::CopyFromTexture(RenderTexture* srcTex)
{
	return false;
}

bool MetalTexture::CopyRectFromTexture(RenderTexture* srcTex, const Rect2Di& srcRect, const Rect2Di& destRect)
{
	return true;
}

void MetalTexture::SetSampleState(RenderSamplerState* state)
{
}

void MetalTexture::ApplyDevice(uint32 slot)
{
}
