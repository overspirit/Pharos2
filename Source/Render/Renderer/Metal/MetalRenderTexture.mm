#include "PreCompile.h"
#include "Pharos.h"

MetalRenderTexture::MetalRenderTexture(id<MTLDevice> device)
{
	m_device = device;
}

MetalRenderTexture::~MetalRenderTexture()
{
}

bool MetalRenderTexture::LoadFromFile(const char8* szPath)
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

bool MetalRenderTexture::LoadFromImage(const Image* image)
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

bool MetalRenderTexture::Create(int32 width, int32 height, EPixelFormat fmt)
{
	return true;
}


bool MetalRenderTexture::MakeColorAttach(int32 width, int32 height, EPixelFormat fmt)
{
	MTLPixelFormat metalFormat = PixelFormat2MetalFormat(fmt);
	MTLTextureDescriptor* textureDesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:metalFormat width:width height:height mipmapped:NO];
	textureDesc.usage = MTLTextureUsageRenderTarget|MTLTextureUsageShaderRead|MTLTextureUsageShaderWrite;
	textureDesc.resourceOptions = MTLResourceStorageModePrivate;
	
	m_texture = [m_device newTextureWithDescriptor:textureDesc];
	
	m_width = width;
	m_height = height;
	m_fmt = fmt;
	
	return true;
}

bool MetalRenderTexture::MakeDepthAttach(int32 width, int32 height, EPixelFormat fmt)
{
	MTLPixelFormat metalFormat = PixelFormat2MetalFormat(fmt);
	MTLTextureDescriptor* textureDesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:metalFormat width:width height:height mipmapped:NO];
	textureDesc.usage = MTLTextureUsageRenderTarget|MTLTextureUsageShaderRead|MTLTextureUsageShaderWrite;
	textureDesc.resourceOptions = MTLResourceStorageModePrivate;
	
	m_texture = [m_device newTextureWithDescriptor:textureDesc];
	
	m_width = width;
	m_height = height;
	m_fmt = fmt;
	
	return true;
}

bool MetalRenderTexture::CopyFromData(const void* pImageData, uint32 nDataSize)
{

	return true;
}

bool MetalRenderTexture::CopyRectFromData(const void* pData, uint32 nDataSize, const Rect2Di& rt)
{
	return true;
}

bool MetalRenderTexture::CopyFromTexture(RenderTexture* srcTex)
{
	return false;
}

bool MetalRenderTexture::CopyRectFromTexture(RenderTexture* srcTex, const Rect2Di& srcRect, const Rect2Di& destRect)
{
	return true;
}

void MetalRenderTexture::SetSampleState(RenderSamplerState* state)
{
}

void MetalRenderTexture::ApplyVertexTexture(uint32 slot, id<MTLRenderCommandEncoder> encoder)
{
	[encoder setVertexTexture:m_texture atIndex:slot];
}

void MetalRenderTexture::ApplyFragmentTexture(uint32 slot, id<MTLRenderCommandEncoder> encoder)
{
	[encoder setFragmentTexture: m_texture atIndex: slot];
}
