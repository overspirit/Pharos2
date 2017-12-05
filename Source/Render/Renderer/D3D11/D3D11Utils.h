﻿#pragma once

#define CHECK_ENUM(i, e) assert(i == e)

namespace Pharos
{
	namespace Render
	{
		//DXGI_FORMAT转换成EPixelFormat
		inline EPixelFormat D3D11FMT2PF(DXGI_FORMAT fmt)
		{
			switch (fmt)
			{
			case DXGI_FORMAT_R32G32B32A32_FLOAT: return EPF_RGBA32_FLOAT;
			case DXGI_FORMAT_R32G32B32A32_UINT: return EPF_RGBA32_UINT;
			case DXGI_FORMAT_R32G32B32A32_SINT: return EPF_RGBA32_SINT;

			case DXGI_FORMAT_R32G32B32_FLOAT: return EPF_RGB32_FLOAT;
			case DXGI_FORMAT_R32G32B32_UINT: return EPF_RGB32_UINT;
			case DXGI_FORMAT_R32G32B32_SINT: return EPF_RGB32_SINT;

			case DXGI_FORMAT_R32G32_FLOAT: return EPF_RG32_FLOAT;
			case DXGI_FORMAT_R32G32_UINT: return EPF_RG32_UINT;
			case DXGI_FORMAT_R32G32_SINT: return EPF_RG32_SINT;

			case DXGI_FORMAT_R32_FLOAT: return EPF_R32_FLOAT;
			case DXGI_FORMAT_R32_UINT: return EPF_R32_UINT;
			case DXGI_FORMAT_R32_SINT: return EPF_R32_SINT;

			case DXGI_FORMAT_R16G16B16A16_FLOAT: return EPF_RGBA16_FLOAT;
			case DXGI_FORMAT_R16G16B16A16_UINT: return EPF_RGBA16_UINT;
			case DXGI_FORMAT_R16G16B16A16_SINT: return EPF_RGBA16_SINT;

			case DXGI_FORMAT_R16G16_FLOAT: return EPF_RG16_FLOAT;
			case DXGI_FORMAT_R16G16_UINT: return EPF_RG16_UINT;
			case DXGI_FORMAT_R16G16_SINT: return EPF_RG16_SINT;

			case DXGI_FORMAT_R16_FLOAT: return EPF_R16_FLOAT;
			case DXGI_FORMAT_R16_UINT: return EPF_R16_UINT;
			case DXGI_FORMAT_R16_SINT: return EPF_R16_SINT;

			case DXGI_FORMAT_R8G8B8A8_UNORM: return EPF_RGBA8_UNORM;
			case DXGI_FORMAT_R8G8B8A8_SNORM: return EPF_RGBA8_SNORM;
			case DXGI_FORMAT_R8G8B8A8_UINT: return EPF_RGBA8_UINT;
			case DXGI_FORMAT_R8G8B8A8_SINT: return EPF_RGBA8_SINT;

			case DXGI_FORMAT_R8G8_UNORM: return EPF_RG8_UNORM;
			case DXGI_FORMAT_R8G8_SNORM: return EPF_RG8_SNORM;
			case DXGI_FORMAT_R8G8_UINT: return EPF_RG8_UINT;
			case DXGI_FORMAT_R8G8_SINT: return EPF_RG8_SINT;

			case DXGI_FORMAT_R8_UNORM: return EPF_R8_UNORM;
			case DXGI_FORMAT_R8_SNORM: return EPF_R8_SNORM;
			case DXGI_FORMAT_R8_UINT: return EPF_R8_UINT;
			case DXGI_FORMAT_R8_SINT: return EPF_R8_SINT;

			case DXGI_FORMAT_A8_UNORM: return EPF_A8_UNORM;

			case DXGI_FORMAT_B8G8R8A8_UNORM: return EPF_BGRA8_UNORM;

			case DXGI_FORMAT_D32_FLOAT: return EPF_D32_FLOAT;
			case DXGI_FORMAT_D24_UNORM_S8_UINT: return EPF_D24_UNORM_S8_UINT;
			case DXGI_FORMAT_D16_UNORM: return EPF_D16_UNORM;
			}

			assert(false);

			return EPF_END;
		}

		//EPixelFormat转换成DXGI_FORMAT
		inline DXGI_FORMAT PF2D3D11FMT(EPixelFormat pf)
		{
			CHECK_ENUM(0, EPF_RGBA32_FLOAT);
			CHECK_ENUM(1, EPF_RGBA32_UINT);
			CHECK_ENUM(2, EPF_RGBA32_SINT);

			CHECK_ENUM(3, EPF_RGB32_FLOAT);
			CHECK_ENUM(4, EPF_RGB32_UINT);
			CHECK_ENUM(5, EPF_RGB32_SINT);

			CHECK_ENUM(6, EPF_RG32_FLOAT);
			CHECK_ENUM(7, EPF_RG32_UINT);
			CHECK_ENUM(8, EPF_RG32_SINT);

			CHECK_ENUM(9, EPF_R32_FLOAT);
			CHECK_ENUM(10, EPF_R32_UINT);
			CHECK_ENUM(11, EPF_R32_SINT);

			CHECK_ENUM(12, EPF_RGBA16_FLOAT);
			CHECK_ENUM(13, EPF_RGBA16_UINT);
			CHECK_ENUM(14, EPF_RGBA16_SINT);

			CHECK_ENUM(15, EPF_RG16_FLOAT);
			CHECK_ENUM(16, EPF_RG16_UINT);
			CHECK_ENUM(17, EPF_RG16_SINT);

			CHECK_ENUM(18, EPF_R16_FLOAT);
			CHECK_ENUM(19, EPF_R16_UINT);
			CHECK_ENUM(20, EPF_R16_SINT);

			CHECK_ENUM(21, EPF_RGBA8_UNORM);
			CHECK_ENUM(22, EPF_RGBA8_SNORM);
			CHECK_ENUM(23, EPF_RGBA8_UINT);
			CHECK_ENUM(24, EPF_RGBA8_SINT);

			CHECK_ENUM(25, EPF_RG8_UNORM);
			CHECK_ENUM(26, EPF_RG8_SNORM);
			CHECK_ENUM(27, EPF_RG8_UINT);
			CHECK_ENUM(28, EPF_RG8_SINT);

			CHECK_ENUM(29, EPF_R8_UNORM);
			CHECK_ENUM(30, EPF_R8_SNORM);
			CHECK_ENUM(31, EPF_R8_UINT);
			CHECK_ENUM(32, EPF_R8_SINT);

			CHECK_ENUM(33, EPF_A8_UNORM);

			CHECK_ENUM(34, EPF_BGRA8_UNORM);

			CHECK_ENUM(35, EPF_D32_FLOAT);
			CHECK_ENUM(36, EPF_D24_UNORM_S8_UINT);
			CHECK_ENUM(37, EPF_D16_UNORM);

			const static DXGI_FORMAT formats[] =
			{
				DXGI_FORMAT_R32G32B32A32_FLOAT,	//EPF_RGBA32_FLOAT;
				DXGI_FORMAT_R32G32B32A32_UINT,	//EPF_RGBA32_UINT;
				DXGI_FORMAT_R32G32B32A32_SINT,	//EPF_RGBA32_SINT;

				DXGI_FORMAT_R32G32B32_FLOAT,	//EPF_RGB32_FLOAT;
				DXGI_FORMAT_R32G32B32_UINT,		//EPF_RGB32_UINT;
				DXGI_FORMAT_R32G32B32_SINT,		//EPF_RGB32_SINT;

				DXGI_FORMAT_R32G32_FLOAT,		//EPF_RG32_FLOAT;
				DXGI_FORMAT_R32G32_UINT,		//EPF_RG32_UINT;
				DXGI_FORMAT_R32G32_SINT,		//EPF_RG32_SINT;

				DXGI_FORMAT_R32_FLOAT,			//EPF_R32_FLOAT;
				DXGI_FORMAT_R32_UINT,			//EPF_R32_UINT;
				DXGI_FORMAT_R32_SINT,			//EPF_R32_SINT;

				DXGI_FORMAT_R16G16B16A16_FLOAT,	//EPF_RGBA16_FLOAT;
				DXGI_FORMAT_R16G16B16A16_UINT,	//EPF_RGBA16_UINT;
				DXGI_FORMAT_R16G16B16A16_SINT,	//EPF_RGBA16_SINT;

				DXGI_FORMAT_R16G16_FLOAT,		//EPF_RG16_FLOAT;
				DXGI_FORMAT_R16G16_UINT,		//EPF_RG16_UINT;
				DXGI_FORMAT_R16G16_SINT,		//EPF_RG16_SINT;

				DXGI_FORMAT_R16_FLOAT,			//EPF_R16_FLOAT;
				DXGI_FORMAT_R16_UINT,			//EPF_R16_UINT;
				DXGI_FORMAT_R16_SINT,			//EPF_R16_SINT;

				DXGI_FORMAT_R8G8B8A8_UNORM,		//EPF_RGBA8_UNORM;
				DXGI_FORMAT_R8G8B8A8_SNORM,		//EPF_RGBA8_SNORM;
				DXGI_FORMAT_R8G8B8A8_UINT,		//EPF_RGBA8_UINT;
				DXGI_FORMAT_R8G8B8A8_SINT,		//EPF_RGBA8_SINT;

				DXGI_FORMAT_R8G8_UNORM,			//EPF_RG8_UNORM;
				DXGI_FORMAT_R8G8_SNORM,			//EPF_RG8_SNORM;
				DXGI_FORMAT_R8G8_UINT,			//EPF_RG8_UINT;
				DXGI_FORMAT_R8G8_SINT,			//EPF_RG8_SINT;

				DXGI_FORMAT_R8_UNORM,			//EPF_R8_UNORM;
				DXGI_FORMAT_R8_SNORM,			//EPF_R8_SNORM;
				DXGI_FORMAT_R8_UINT,			//EPF_R8_UINT;
				DXGI_FORMAT_R8_SINT,			//EPF_R8_SINT;

				DXGI_FORMAT_A8_UNORM,			//EPF_A8_UNORM;

				DXGI_FORMAT_B8G8R8A8_UNORM,			//EPF_BGRA8_UNORM;

				DXGI_FORMAT_D32_FLOAT,			//EPF_D32_FLOAT;
				DXGI_FORMAT_D24_UNORM_S8_UINT,	//EPF_D24_UNORM_S8_UINT;
				DXGI_FORMAT_D16_UNORM,			//EPF_D16_UNORM;
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

			case EPF_RGB32_FLOAT:
			case EPF_RGB32_UINT:
			case EPF_RGB32_SINT:
				return 12;

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