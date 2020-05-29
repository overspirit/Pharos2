#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderTexture::VulkanRenderTexture()
{
}

VulkanRenderTexture::~VulkanRenderTexture()
{
}

bool VulkanRenderTexture::LoadFromFile(const char8* szPath)
{
	return false;
}

bool VulkanRenderTexture::LoadFromImage(const Image* image)
{
	return true;
}

bool VulkanRenderTexture::Create(int32 width, int32 height, EPixelFormat fmt)
{
	return true;
}

//bool VulkanRenderTexture::CreateDepthTexture(int32 width, int32 height)
//{
//
//    return true;
//}
//
//bool VulkanRenderTexture::CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt)
//{
//    return true;
//}


bool VulkanRenderTexture::CopyFromData(const void* pImageData, uint32 nDataSize)
{

	return true;
}

bool VulkanRenderTexture::CopyRectFromData(const void* pData, uint32 nDataSize, const Rect2Di& rt)
{
	return true;
}

bool VulkanRenderTexture::CopyFromTexture(RenderTexture* srcTex)
{
	return false;
}

bool VulkanRenderTexture::CopyRectFromTexture(RenderTexture* srcTex, const Rect2Di& srcRect, const Rect2Di& destRect)
{
	return true;
}

void VulkanRenderTexture::SetSampleState(RenderSamplerState* state)
{
}

void VulkanRenderTexture::ApplyDevice(uint32 slot)
{
}
