#pragma once

namespace Pharos
{
	namespace Render
	{
		enum EBlendFunc
		{
			BLEND_ZERO,
			BLEND_ONE,
			BLEND_SRC_COLOR,
			BLEND_INV_SRC_COLOR,
			BLEND_SRC_ALPHA,
			BLEND_INV_SRC_ALPHA,
			BLEND_DEST_COLOR,
			BLEND_INV_DEST_COLOR,
			BLEND_DEST_ALPHA,
			BLEND_INV_DEST_ALPHA,
		};

		enum EBlendOP
		{
			BLEND_OP_ADD,
			BLEND_OP_SUBTRACT,
			BLEND_OP_REV_SUBTRACT,
			BLEND_OP_MIN,
			BLEND_OP_MAX,
		};

		enum EFillMode
		{
			FILL_WIREFRAME,
			FILL_SOLID,
		};

		enum ECullMode
		{
			CULL_NONE,
			CULL_FRONT,
			CULL_BACK,
		};

		enum EDepthWriteType
		{
			DEPTH_WRITE_ZERO,
			DEPTH_WRITE_ALL,
		};

		enum EStencilOp
		{
			STENCIL_OP_KEEP,
			STENCIL_OP_ZERO,
			STENCIL_OP_REPLACE,
			STENCIL_OP_INCR_SAT,
			STENCIL_OP_DECR_SAT,
			STENCIL_OP_INVERT,
			STENCIL_OP_INCR,
			STENCIL_OP_DECR,
		};

		enum EComparisonFunc
		{
			COMPARISON_NEVER,
			COMPARISON_LESS,
			COMPARISON_EQUAL,
			COMPARISON_LESS_EQUAL,
			COMPARISON_GREATER,
			COMPARISON_NOT_EQUAL,
			COMPARISON_GREATER_EQUAL,
			COMPARISON_ALWAYS,
		};

		enum EFilterType
		{
			FILTER_MIN_MAG_MIP_POINT,
			FILTER_MIN_MAG_POINT_MIP_LINEAR,
			FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
			FILTER_MIN_POINT_MAG_MIP_LINEAR,
			FILTER_MIN_LINEAR_MAG_MIP_POINT,
			FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
			FILTER_MIN_MAG_LINEAR_MIP_POINT,
			FILTER_MIN_MAG_MIP_LINEAR,
			FILTER_ANISOTROPIC,
		};

		enum ETextureAddressMode
		{
			TEXTURE_ADDRESS_WRAP,
			TEXTURE_ADDRESS_MIRROR,
			TEXTURE_ADDRESS_CLAMP,
			TEXTURE_ADDRESS_BORDER,
		};

		struct BlendStateDesc
		{
			BlendStateDesc()
			{
				blendEnable = false;
				alphaToCoverageEnable = false;
				srcBlend = BLEND_ONE;
				destBlend = BLEND_ZERO;
				blendOp = BLEND_OP_ADD;
				srcBlendAlpha = BLEND_ONE;
				destBlendAlpha = BLEND_ZERO;
				blendOpAlpha = BLEND_OP_ADD;
			}

			bool blendEnable;
			bool alphaToCoverageEnable;
			EBlendFunc srcBlend;
			EBlendFunc destBlend;
			EBlendOP blendOp;
			EBlendFunc srcBlendAlpha;
			EBlendFunc destBlendAlpha;
			EBlendOP blendOpAlpha;
		};

		struct RasterizerStateDesc
		{
			RasterizerStateDesc()
			{
				fillMode = FILL_SOLID;
				cullMode = CULL_BACK;
				frontCounterClockwise = false;
				depthBias = 0;
				depthBiasClamp = 0;
				slopeScaledDepthBias = 0;
				depthClipEnable = true;
				scissorEnable = false;
				multisampleEnable = false;
				antialiasedLineEnable = false;
			}

			EFillMode fillMode;
			ECullMode cullMode;
			bool frontCounterClockwise;
			int32 depthBias;
			float32 depthBiasClamp;
			float32 slopeScaledDepthBias;
			bool depthClipEnable;
			bool scissorEnable;
			bool multisampleEnable;
			bool antialiasedLineEnable;
		};

		struct DepthStencilStateDesc
		{
			DepthStencilStateDesc()
			{
				depthEnable = true;
				depthWriteMask = DEPTH_WRITE_ALL;
				depthFunc = COMPARISON_LESS;
				stencilEnable = false;
				stencilRefValue = 0;
				stencilWriteMask = 0xFF;
				frontStencilFailOp = STENCIL_OP_KEEP;
				frontStencilDepthFailOp = STENCIL_OP_KEEP;
				frontStencilPassOp = STENCIL_OP_KEEP;
				frontStencilFunc = COMPARISON_ALWAYS;
				backStencilFailOp = STENCIL_OP_KEEP;
				backStencilDepthFailOp = STENCIL_OP_KEEP;
				backStencilPassOp = STENCIL_OP_KEEP;
				backStencilFunc = COMPARISON_ALWAYS;
			}

			bool depthEnable;
			EDepthWriteType depthWriteMask;
			EComparisonFunc depthFunc;
			bool stencilEnable;
			uint32 stencilRefValue;
			uint8 stencilWriteMask;
			EStencilOp frontStencilFailOp;
			EStencilOp frontStencilDepthFailOp;
			EStencilOp frontStencilPassOp;
			EComparisonFunc frontStencilFunc;
			EStencilOp backStencilFailOp;
			EStencilOp backStencilDepthFailOp;
			EStencilOp backStencilPassOp;
			EComparisonFunc backStencilFunc;
		};

		struct SamplerStateDesc
		{
			SamplerStateDesc()
			{
				filter = FILTER_MIN_MAG_MIP_LINEAR;
				addressU = TEXTURE_ADDRESS_CLAMP;
				addressV = TEXTURE_ADDRESS_CLAMP;
				addressW = TEXTURE_ADDRESS_CLAMP;
				mipLODBias = 0;
				maxAnisotropy = 16;
				comparisonFunc = COMPARISON_NEVER;
				minLOD = -FLT_MAX;
				maxLOD = FLT_MAX;
			}

			EFilterType filter;
			ETextureAddressMode addressU;
			ETextureAddressMode addressV;
			ETextureAddressMode addressW;
			float32 mipLODBias;
			uint32 maxAnisotropy;
			EComparisonFunc comparisonFunc;
			Color4f borderColor;
			float32 minLOD;
			float32 maxLOD;
		};

	}
}