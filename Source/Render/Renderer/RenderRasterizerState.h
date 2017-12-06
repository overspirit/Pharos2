#pragma once

namespace Pharos
{
	namespace Render
	{
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

		class RenderRasterizerState
		{
		public:
			RenderRasterizerState()
			{

			}

			virtual ~RenderRasterizerState(void)
			{

			}

		protected:
			RasterizerStateDesc		m_desc;

		public:
			virtual RenderRasterizerState* Clone() = 0;

			virtual EFillMode GetFillMode() const { return m_desc.fillMode; }
			virtual ECullMode GetCullMode() const { return m_desc.cullMode; }
			virtual bool IsFrontCounterClockwise() const { return m_desc.frontCounterClockwise; }
			virtual int32 GetDepthBias() const { return m_desc.depthBias; }
			virtual float32 GetDepthBiasClamp() const { return m_desc.depthBiasClamp; }
			virtual float32 GetSlopeScaledDepthBias() const { return m_desc.slopeScaledDepthBias; }
			virtual bool IsDepthClipEnable() const { return m_desc.depthClipEnable; }
			virtual bool IsScissorEnable() const { return m_desc.scissorEnable; }
			virtual bool IsMultisampleEnable() const { return m_desc.multisampleEnable; }
			virtual bool IsAntialiasedLineEnable() const { return m_desc.antialiasedLineEnable; }
		};
	}
}