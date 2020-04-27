#include "PreCompile.h"
#include "Pharos.h"

MetalTexture::MetalTexture(id<MTLDevice> device)
{
	m_device = device;
}

MetalTexture::~MetalTexture()
{
}

bool MetalTexture::LoadFromFile(const char8* szPath)
{
	if (szPath == nullptr || *szPath == '\0') return false;

	Image* image = sResMgr->GenerateImage(szPath);
	if (image == nullptr) return false;

	if (image->GetBPP() != 32)
	{
		if (!image->ConvertTo32Bits()) return false;
	}

	return LoadFromImage(image);
}

bool MetalTexture::LoadFromImage(const Image* image)
{
	uint32 width = image->GetWidth();
	uint32 height = image->GetHeight();

	// 纹理描述符
	MTLTextureDescriptor* textureDescriptor = [[MTLTextureDescriptor alloc] init];
	textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
	textureDescriptor.width = width;
	textureDescriptor.height = height;
	m_texture = [m_device newTextureWithDescriptor : textureDescriptor]; // 创建纹理

	MTLRegion region = { { 0, 0, 0 }, {width, height, 1} }; // 纹理上传的范围
	Byte* imageBytes = (Byte*)image->GetDataPointer();
	if (imageBytes)
	{
		[m_texture replaceRegion : region mipmapLevel : 0 withBytes : imageBytes bytesPerRow : 4 * width];
	}

	return true;
}

bool MetalTexture::Create(int32 width, int32 height, EPixelFormat fmt)
{
	return true;
}

//bool MetalTexture::CreateDepthTexture(int32 width, int32 height)
//{
//
//    return true;
//}
//
//bool MetalTexture::CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt)
//{
//    return true;
//}


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
