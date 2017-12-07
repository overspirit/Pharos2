﻿#include "PreCompile.h"
#include "CoreGlobal.h"

Image::Image()
{
    m_strResType = "IMAGE";
    
    m_dib = nullptr;
}

Image::~Image()
{
	if (m_dib != nullptr)
	{
		FreeImage_Unload(m_dib);
		m_dib = nullptr;
	}
}

bool Image::CreateImage(int32 width, int32 height)
{
	m_dib = FreeImage_Allocate(width, height, 32);
	if (m_dib == nullptr) return false;

	return true;
}

bool Image::Open(const char8* path)
{
	m_file = sKernel->OpenFileStream(path);
	if (m_file == nullptr) return false;
    return true;
}

bool Image::Save(const char8* path)
{
	string savePath = m_file->GetPath();
	if (path != nullptr) savePath = path;

	savePath += ".bmp";

	//???没有通过file接口保存，需要指定回调
	return (FreeImage_Save(FIF_BMP, m_dib, savePath.c_str()) == TRUE);
}

bool Image::LoadImageFile()
{
//    FreeImageIO io;
//    io.read_proc = FI_Read;
//    io.write_proc = FI_Write;
//    io.seek_proc = FI_Seek;
//    io.tell_proc = FI_Tell;
//    
//    FreeImage_GetFileTypeFromHandle(&io, (void*)0x12345678);
    
    //FreeImage_LoadFromHandle()
    uint32 fileSize = m_file->GetSize();
    
	MemoryBuffer memBuf(fileSize);
    char8* data = (char8*)memBuf.GetPointer();
    m_file->Read(data, fileSize);
    
    FIMEMORY* fiMem = FreeImage_OpenMemory((BYTE*)data, fileSize);
    
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(fiMem, 0);
    
    if (FreeImage_FIFSupportsReading(fif))
    {
        m_dib = FreeImage_LoadFromMemory(fif, fiMem);
        
        if (m_dib != nullptr)
        {
            FreeImage_FlipVertical(m_dib);
            
            m_strFilePath = m_file->GetPath();
            
            FreeImage_CloseMemory(fiMem);
            
            return true;
        }
    }
    
    FreeImage_CloseMemory(fiMem);
    
    return false;
}

uint32 Image::GetWidth() const
{
    if(m_dib == nullptr) return 0;
    return FreeImage_GetWidth(m_dib);
}

uint32 Image::GetHeight() const
{
    if(m_dib == nullptr) return 0;
    return FreeImage_GetHeight(m_dib);
}

uint32 Image::GetBPP() const
{
    if(m_dib == nullptr) return 0;
    return FreeImage_GetBPP(m_dib);
}

const void* Image::GetDataPointer() const
{
    if(m_dib == nullptr) return nullptr;
    return FreeImage_GetBits(m_dib);
}

uint32 Image::GetPitch() const
{
    if(m_dib == nullptr) return 0;
    return FreeImage_GetPitch(m_dib);
}

bool Image::ConvertTo32Bits()
{
    if(m_dib == nullptr) return false;
    FIBITMAP* dib = FreeImage_ConvertTo32Bits(m_dib);
    if(dib == nullptr) return false;
    FreeImage_Unload(m_dib);
    m_dib = dib;
    return true;
}

bool Image::ConvertTo24Bits()
{
    if(m_dib == nullptr) return false;
    FIBITMAP* dib = FreeImage_ConvertTo24Bits(m_dib);
    if(dib == nullptr) return false;
    FreeImage_Unload(m_dib);
    m_dib = dib;
    return true;
}