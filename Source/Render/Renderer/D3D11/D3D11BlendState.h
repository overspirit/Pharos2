#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11BlendState : public RenderBlendState
		{
		public:
			D3D11BlendState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
			virtual ~D3D11BlendState(void);

		private:
			ID3D11Device*			m_pDevice;
			ID3D11DeviceContext*	m_pContext;
			
			ID3D11BlendState*	m_pState;
			
		public:
			virtual bool CreateState(const BlendStateDesc& desc);

			virtual RenderBlendState* Clone();

			virtual void ApplyDevice();
		};
	}
}