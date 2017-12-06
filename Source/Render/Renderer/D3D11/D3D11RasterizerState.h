#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11RasterizerState : public RenderRasterizerState
		{
		public:
			D3D11RasterizerState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
			virtual ~D3D11RasterizerState(void);

		private:
			ID3D11Device*			m_pDevice;
			ID3D11DeviceContext*	m_pContext;

			ID3D11RasterizerState*	m_pState;

		public:
			virtual bool CreateState(const RasterizerStateDesc& desc);

			virtual RenderRasterizerState* Clone();

			virtual void ApplyDevice();
		};
	}
}