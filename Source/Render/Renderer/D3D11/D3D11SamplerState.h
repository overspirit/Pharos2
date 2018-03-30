#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11SamplerState : public RenderSamplerState
		{
		public:
			D3D11SamplerState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
			virtual ~D3D11SamplerState(void);

		private:
			ID3D11Device*			m_pDevice;
			ID3D11DeviceContext*	m_pContext;

			ID3D11SamplerState*		m_pState;

		public:
			virtual bool CreateState(const SamplerStateDesc& desc);

			virtual RenderSamplerState* Clone();

			virtual void ApplyDevice(uint32 slot);
		};
	}
}