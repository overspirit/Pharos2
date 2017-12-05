#pragma once

namespace Pharos
{
	namespace Render
	{
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

		class RenderDepthStencilState
		{
		public:
			RenderDepthStencilState()
			{

			}

			virtual ~RenderDepthStencilState(void)
			{

			}

		protected:
			DepthStencilStateDesc		m_desc;

		public:
			virtual bool IsDepthEnable() const { return m_desc.depthEnable; }
			virtual EDepthWriteType GetDepthWriteMask() const { return m_desc.depthWriteMask; }
			virtual EComparisonFunc GetDepthFunc() const { return m_desc.depthFunc; }
			virtual bool IsStencilEnable() const { return m_desc.stencilEnable; };
			virtual uint32 GetStencilRefValue() const { return m_desc.stencilRefValue; }
			virtual uint8 GetStencilWriteMask() const { return m_desc.stencilWriteMask; }
			virtual EStencilOp GetFrontStencilFailOp() const { return m_desc.frontStencilFailOp; }
			virtual EStencilOp GetFrontStencilDepthFailOp() const { return m_desc.frontStencilDepthFailOp; }
			virtual EStencilOp GetFrontStencilPassOp() const { return m_desc.frontStencilPassOp; }
			virtual EComparisonFunc GetFrontStencilFunc() const { return m_desc.frontStencilFunc; }
			virtual EStencilOp GetBackStencilFailOp() const { return m_desc.backStencilFailOp; }
			virtual EStencilOp GetBackStencilDepthFailOp() const { return m_desc.backStencilDepthFailOp; }
			virtual EStencilOp GetBackStencilPassOp() const { return m_desc.backStencilPassOp; }
			virtual EComparisonFunc GetBackStencilFunc() const { return m_desc.backStencilFunc; }
		};
	}
}