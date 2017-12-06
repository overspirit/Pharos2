#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11DepthStencilState : public RenderDepthStencilState
		{
		public:
			D3D11DepthStencilState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
			virtual ~D3D11DepthStencilState(void);

		private:
			ID3D11Device*			m_pDevice;
			ID3D11DeviceContext*	m_pContext;

			ID3D11DepthStencilState*	m_pState;

		public:
			virtual bool CreateState(const DepthStencilStateDesc& desc);
			
			virtual RenderDepthStencilState* Clone();

			virtual void ApplyDevice();
		};
	}
}