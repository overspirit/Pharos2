#pragma once

#define CHECK_ENUM(i, e) assert(i == e)

namespace Pharos
{
	namespace Render
	{
		//MTLPixelFormat转换成EPixelFormat
		inline EPixelFormat MetalFormat2PixelFormat(MTLPixelFormat fmt)
		{
			switch (fmt)
			{
			case MTLPixelFormatInvalid: return EPF_END;
					
			case MTLPixelFormatRGBA32Float: return EPF_RGBA32_FLOAT;
			case MTLPixelFormatRGBA32Uint: return EPF_RGBA32_UINT;
			case MTLPixelFormatRGBA32Sint: return EPF_RGBA32_SINT;

			case MTLPixelFormatRG32Float: return EPF_RG32_FLOAT;
			case MTLPixelFormatRG32Uint: return EPF_RG32_UINT;
			case MTLPixelFormatRG32Sint: return EPF_RG32_SINT;

			case MTLPixelFormatR32Float: return EPF_R32_FLOAT;
			case MTLPixelFormatR32Uint: return EPF_R32_UINT;
			case MTLPixelFormatR32Sint: return EPF_R32_SINT;

			case MTLPixelFormatRGBA16Float: return EPF_RGBA16_FLOAT;
			case MTLPixelFormatRGBA16Uint: return EPF_RGBA16_UINT;
			case MTLPixelFormatRGBA16Sint: return EPF_RGBA16_SINT;

			case MTLPixelFormatRG16Float: return EPF_RG16_FLOAT;
			case MTLPixelFormatRG16Uint: return EPF_RG16_UINT;
			case MTLPixelFormatRG16Sint: return EPF_RG16_SINT;

			case MTLPixelFormatR16Float: return EPF_R16_FLOAT;
			case MTLPixelFormatR16Uint: return EPF_R16_UINT;
			case MTLPixelFormatR16Sint: return EPF_R16_SINT;
					
			case MTLPixelFormatRGBA8Unorm: return EPF_RGBA8_UNORM;
			case MTLPixelFormatRGBA8Snorm: return EPF_RGBA8_SNORM;
			case MTLPixelFormatRGBA8Uint: return EPF_RGBA8_UINT;
			case MTLPixelFormatRGBA8Sint: return EPF_RGBA8_SINT;

			case MTLPixelFormatRG8Unorm: return EPF_RG8_UNORM;
			case MTLPixelFormatRG8Snorm: return EPF_RG8_SNORM;
			case MTLPixelFormatRG8Uint: return EPF_RG8_UINT;
			case MTLPixelFormatRG8Sint: return EPF_RG8_SINT;

			case MTLPixelFormatR8Unorm: return EPF_R8_UNORM;
			case MTLPixelFormatR8Snorm: return EPF_R8_SNORM;
			case MTLPixelFormatR8Uint: return EPF_R8_UINT;
			case MTLPixelFormatR8Sint: return EPF_R8_SINT;

			case MTLPixelFormatA8Unorm: return EPF_A8_UNORM;

			case MTLPixelFormatBGRA8Unorm: return EPF_BGRA8_UNORM;
			case MTLPixelFormatBGRA8Unorm_sRGB: return EPF_BGRA8_UNORM_SRGB;
					
			case MTLPixelFormatDepth32Float_Stencil8: return EPF_D32_FLOAT_S8_UINT;
			case MTLPixelFormatDepth32Float: return EPF_D32_FLOAT;
			case MTLPixelFormatDepth24Unorm_Stencil8: return EPF_D24_UNORM_S8_UINT;
			case MTLPixelFormatDepth16Unorm: return EPF_D16_UNORM;
			}

			assert(false);

			return EPF_END;
		}

		//EPixelFormat转换成DXGI_FORMAT
		inline MTLPixelFormat PixelFormat2MetalFormat(EPixelFormat pf)
		{
			CHECK_ENUM(0, EPF_RGBA32_FLOAT);
			CHECK_ENUM(1, EPF_RGBA32_UINT);
			CHECK_ENUM(2, EPF_RGBA32_SINT);

			CHECK_ENUM(3, EPF_RG32_FLOAT);
			CHECK_ENUM(4, EPF_RG32_UINT);
			CHECK_ENUM(5, EPF_RG32_SINT);

			CHECK_ENUM(6, EPF_R32_FLOAT);
			CHECK_ENUM(7, EPF_R32_UINT);
			CHECK_ENUM(8, EPF_R32_SINT);

			CHECK_ENUM(9, EPF_RGBA16_FLOAT);
			CHECK_ENUM(10, EPF_RGBA16_UINT);
			CHECK_ENUM(11, EPF_RGBA16_SINT);

			CHECK_ENUM(12, EPF_RG16_FLOAT);
			CHECK_ENUM(13, EPF_RG16_UINT);
			CHECK_ENUM(14, EPF_RG16_SINT);

			CHECK_ENUM(15, EPF_R16_FLOAT);
			CHECK_ENUM(16, EPF_R16_UINT);
			CHECK_ENUM(17, EPF_R16_SINT);

			CHECK_ENUM(18, EPF_RGBA8_UNORM);
			CHECK_ENUM(19, EPF_RGBA8_SNORM);
			CHECK_ENUM(20, EPF_RGBA8_UINT);
			CHECK_ENUM(21, EPF_RGBA8_SINT);

			CHECK_ENUM(22, EPF_RG8_UNORM);
			CHECK_ENUM(23, EPF_RG8_SNORM);
			CHECK_ENUM(24, EPF_RG8_UINT);
			CHECK_ENUM(25, EPF_RG8_SINT);

			CHECK_ENUM(26, EPF_R8_UNORM);
			CHECK_ENUM(27, EPF_R8_SNORM);
			CHECK_ENUM(28, EPF_R8_UINT);
			CHECK_ENUM(29, EPF_R8_SINT);

			CHECK_ENUM(30, EPF_A8_UNORM);

			CHECK_ENUM(31, EPF_BGRA8_UNORM);
			CHECK_ENUM(32, EPF_BGRA8_UNORM_SRGB);

			CHECK_ENUM(33, EPF_D32_FLOAT_S8_UINT);
			CHECK_ENUM(34, EPF_D32_FLOAT);
			CHECK_ENUM(35, EPF_D24_UNORM_S8_UINT);
			CHECK_ENUM(36, EPF_D16_UNORM);

			const static MTLPixelFormat formats[] =
			{
				MTLPixelFormatRGBA32Float, //EPF_RGBA32_FLOAT
				MTLPixelFormatRGBA32Uint, //EPF_RGBA32_UINT
				MTLPixelFormatRGBA32Sint, //EPF_RGBA32_SINT
				
				MTLPixelFormatRG32Float, //EPF_RG32_FLOAT
				MTLPixelFormatRG32Uint, //EPF_RG32_UINT
				MTLPixelFormatRG32Sint, //EPF_RG32_SINT
				
				MTLPixelFormatR32Float, //EPF_R32_FLOAT
				MTLPixelFormatR32Uint, //EPF_R32_UINT
				MTLPixelFormatR32Sint, //EPF_R32_SINT
				
				MTLPixelFormatRGBA16Float, //EPF_RGBA16_FLOAT
				MTLPixelFormatRGBA16Uint, //EPF_RGBA16_UINT
				MTLPixelFormatRGBA16Sint, //EPF_RGBA16_SINT
				
				MTLPixelFormatRG16Float, //EPF_RG16_FLOAT
				MTLPixelFormatRG16Uint, //EPF_RG16_UINT
				MTLPixelFormatRG16Sint, //EPF_RG16_SINT
				
				MTLPixelFormatR16Float, //EPF_R16_FLOAT
				MTLPixelFormatR16Uint, //EPF_R16_UINT
				MTLPixelFormatR16Sint, //EPF_R16_SINT
				
				MTLPixelFormatRGBA8Unorm, //EPF_RGBA8_UNORM
				MTLPixelFormatRGBA8Snorm, //EPF_RGBA8_SNORM
				MTLPixelFormatRGBA8Uint, //EPF_RGBA8_UINT
				MTLPixelFormatRGBA8Sint, //EPF_RGBA8_SINT
				
				MTLPixelFormatRG8Unorm, //EPF_RG8_UNORM
				MTLPixelFormatRG8Snorm, //EPF_RG8_SNORM
				MTLPixelFormatRG8Uint, //EPF_RG8_UINT
				MTLPixelFormatRG8Sint, //EPF_RG8_SINT
				
				MTLPixelFormatR8Unorm, //EPF_R8_UNORM
				MTLPixelFormatR8Snorm, //EPF_R8_SNORM
				MTLPixelFormatR8Uint, //EPF_R8_UINT
				MTLPixelFormatR8Sint, //EPF_R8_SINT
				
				MTLPixelFormatA8Unorm, //EPF_A8_UNORM
				
				MTLPixelFormatBGRA8Unorm, //EPF_BGRA8_UNORM
				MTLPixelFormatBGRA8Unorm_sRGB, //EPF_BGRA8_UNORM_SRGB
				
				MTLPixelFormatDepth32Float_Stencil8, //EPF_D32_FLOAT_S8_UINT
				MTLPixelFormatDepth32Float, //EPF_D32_FLOAT
				MTLPixelFormatDepth24Unorm_Stencil8, //EPF_D24_UNORM_S8_UINT
				MTLPixelFormatDepth16Unorm, //EPF_D16_UNORM
			};

			return formats[pf];
		}

		//获取EPixelFormat数据大小
		inline uint32 GetPFSize(EPixelFormat pf)
		{
			switch (pf)
			{
			case EPF_RGBA32_FLOAT:
			case EPF_RGBA32_UINT:
			case EPF_RGBA32_SINT:
				return 16;

			case EPF_RG32_FLOAT:
			case EPF_RG32_UINT:
			case EPF_RG32_SINT:
				return 8;

			case EPF_R32_FLOAT:
			case EPF_R32_UINT:
			case EPF_R32_SINT:
				return 4;

			case EPF_RGBA16_FLOAT:
			case EPF_RGBA16_UINT:
			case EPF_RGBA16_SINT:
				return 8;

			case EPF_RG16_FLOAT:
			case EPF_RG16_UINT:
			case EPF_RG16_SINT:
				return 4;

			case EPF_R16_FLOAT:
			case EPF_R16_UINT:
			case EPF_R16_SINT:
				return 2;

			case EPF_RGBA8_UNORM:
			case EPF_RGBA8_SNORM:
			case EPF_RGBA8_UINT:
			case EPF_RGBA8_SINT:
				return 4;

			case EPF_RG8_UNORM:
			case EPF_RG8_SNORM:
			case EPF_RG8_UINT:
			case EPF_RG8_SINT:
				return 2;

			case EPF_R8_UNORM:
			case EPF_R8_SNORM:
			case EPF_R8_UINT:
			case EPF_R8_SINT:
				return 1;

			case EPF_A8_UNORM:
				return 1;

			case EPF_BGRA8_UNORM:
				return 4;

			case EPF_D32_FLOAT: return 4;
			case EPF_D24_UNORM_S8_UINT: return 4;
			case EPF_D16_UNORM: return 2;
			}

			assert(false);

			return 0;
		}
	}
}
