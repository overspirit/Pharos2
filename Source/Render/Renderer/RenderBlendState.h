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

		class RenderBlendState
		{
		public:
			RenderBlendState()
			{

			}

			virtual ~RenderBlendState(void)
			{

			}

		protected:
			BlendStateDesc	m_desc;

		public:
			virtual RenderBlendState* Clone() = 0;

			virtual bool IsBlendEnable() const { return m_desc.blendEnable; }
			virtual bool IsAlphaToCoverageEnable() const { return m_desc.alphaToCoverageEnable; }
			virtual EBlendFunc GetSrcBlend() const { return m_desc.srcBlend; }
			virtual EBlendFunc GetDestBlend() const { return m_desc.destBlend; }
			virtual EBlendOP GetBlendOp() const { return m_desc.blendOp; }
			virtual EBlendFunc GetSrcBlendAlpha() const { return m_desc.srcBlendAlpha; }
			virtual EBlendFunc GetDestBlendAlpha() const { return m_desc.destBlendAlpha; }
			virtual EBlendOP GetBlendOpAlpha() const { return m_desc.blendOpAlpha; }
		};
	}
}