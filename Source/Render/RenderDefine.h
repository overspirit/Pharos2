#pragma once

namespace Pharos
{
	namespace Render
	{
		enum DrawType
		{
			EDT_POINTLIST,
			EDT_LINELIST,
			EDT_LINESTRIP,
			EDT_TRIANGLELIST,
			EDT_TRIANGLESTRIP,
		};

		//抗锯齿级别
		enum EMultisampleType
		{
			EMT_None = 0,
			EMT_2x = 2,
			EMT_4x = 4,
			EMT_8x = 8,
			EMT_16x = 16
		};

		enum EBufferType
		{
			EB_Static,
			EB_Dynamic,
		};

		//像素格式
		enum EPixelFormat
		{
			EPF_RGBA32_FLOAT,
			EPF_RGBA32_UINT,
			EPF_RGBA32_SINT,

			EPF_RG32_FLOAT,
			EPF_RG32_UINT,
			EPF_RG32_SINT,

			EPF_R32_FLOAT,
			EPF_R32_UINT,
			EPF_R32_SINT,

			EPF_RGBA16_FLOAT,
			EPF_RGBA16_UINT,
			EPF_RGBA16_SINT,

			EPF_RG16_FLOAT,
			EPF_RG16_UINT,
			EPF_RG16_SINT,

			EPF_R16_FLOAT,
			EPF_R16_UINT,
			EPF_R16_SINT,

			EPF_RGBA8_UNORM,
			EPF_RGBA8_SNORM,
			EPF_RGBA8_UINT,
			EPF_RGBA8_SINT,

			EPF_RG8_UNORM,
			EPF_RG8_SNORM,
			EPF_RG8_UINT,
			EPF_RG8_SINT,

			EPF_R8_UNORM,
			EPF_R8_SNORM,
			EPF_R8_UINT,
			EPF_R8_SINT,

			EPF_A8_UNORM,

            EPF_BGRA8_UNORM,
			EPF_BGRA8_UNORM_SRGB,
			
			EPF_D32_FLOAT_S8_UINT,
			EPF_D32_FLOAT,
			EPF_D24_UNORM_S8_UINT,
			EPF_D16_UNORM,

			EPF_END,
		};

		struct RenderParam
		{
			RenderParam()
			{
				width = 0;
				height = 0;
				sampleType = EMT_None;
				sync = false;
				fullScreen = false;

				hdrEnabled = true;
				gammaEnabled = true;

				backColor = 0xFF000000;
			}

			int32				width;
			int32				height;
			Color4				backColor;
			EMultisampleType	sampleType;
			bool				sync;
			bool				fullScreen;

			bool				hdrEnabled;
			bool				gammaEnabled;
		};
	}
}
