#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11ConstantBuffer : public RenderShaderData
		{
		public:
			D3D11ConstantBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
			virtual ~D3D11ConstantBuffer();

		private:
			ID3D11Device*			m_pDevice;
			ID3D11DeviceContext*	m_pContext;

			ID3D11Buffer*			m_pBuffer;

			MemoryBuffer			m_data;

		private:
			bool CreateBuffer(uint32 bufSize);

		public:
			virtual void CopyData(const MemoryBuffer& data, uint32 offset = 0);
			virtual void CopyData(const void* data, uint32 len, uint32 offset = 0);

			virtual void ApplyDevice(uint32 slot);
		};
	}
}