#pragma once

namespace Pharos
{
	namespace Render
	{
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

		class RenderSamplerState
		{
		public:
			RenderSamplerState()
			{

			}

			virtual ~RenderSamplerState(void)
			{

			}

		protected:
			SamplerStateDesc		m_desc;

		public:
			virtual RenderSamplerState* Clone() = 0;

			virtual EFilterType GetFilter() const { return m_desc.filter; }
			virtual ETextureAddressMode GetAddressU() const { return m_desc.addressU; }
			virtual ETextureAddressMode GetAddressV() const { return m_desc.addressV; }
			virtual ETextureAddressMode GetAddressW() const { return m_desc.addressW; }
			virtual float32 GetMipLODBias() const { return m_desc.mipLODBias; }
			virtual uint32 GetMaxAnisotropy() const { return m_desc.maxAnisotropy; }
			virtual EComparisonFunc GetComparisonFunc() const { return m_desc.comparisonFunc; }
			virtual Color4f GetBorderColor() const { return m_desc.borderColor; }
			virtual float32 GetMinLOD() const { return m_desc.minLOD; }
			virtual float32 GetMaxLOD() const { return m_desc.maxLOD; }
		};
	}
}