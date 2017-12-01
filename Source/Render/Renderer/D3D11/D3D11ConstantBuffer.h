#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11ConstantBuffer// : public IRenderShaderData
		{
		public:
			D3D11ConstantBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
			virtual ~D3D11ConstantBuffer();

		private:
			ID3D11Device*			m_pDevice;
			ID3D11DeviceContext*	m_pContext;

			ID3D11Buffer*			m_pBuffer;

			MemoryBuffer			m_data;

		public:
			virtual void CopyData(const MemoryBuffer& data);
			virtual void CopyData(const void* data, uint32 len);

			virtual void ApplyToDevice(uint32 slot);
		};
	}

	typedef shared_ptr<Render::D3D11ConstantBuffer> D3D11ConstantBufferPtr;
}