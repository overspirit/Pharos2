#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11ShaderProgram : public RenderProgram
		{
		public:
			D3D11ShaderProgram(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
			virtual ~D3D11ShaderProgram();

		private:
			ID3D11Device*			m_pDevice;
			ID3D11DeviceContext*	m_pContext;

			ID3D10Blob*				m_pVertexBlob;

			ID3D11VertexShader*		m_pVertexShader;
			ID3D11PixelShader*		m_pPixelShader;
			ID3D11HullShader*		m_pHullShader;
			ID3D11DomainShader*		m_pDomainShader;
			ID3D11GeometryShader*	m_pGeometryShader;
			ID3D11ComputeShader*	m_pComputeShader;

			string					m_strError;
			
			string					m_vertShaderText;
			string					m_vertShaderEnter;
			string					m_pixelShaderText;
			string					m_pixelShaderEnter;

		public:
			ID3D10Blob* GetVertexBlob() { return m_pVertexBlob; }

		public:
			virtual bool CompileVertexShader(const char8* shader, const char8* enter);
			virtual bool CompilePixelShader(const char8* shader, const char8* enter);

			virtual RenderProgram* Clone();

			virtual void ApplyDevice();
		};
	}
}