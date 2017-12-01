#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11ShaderProgram::D3D11ShaderProgram(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	m_pVertexBlob = nullptr;

	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pHullShader = nullptr;
	m_pDomainShader = nullptr;
	m_pGeometryShader = nullptr;
	m_pComputeShader = nullptr;
}

D3D11ShaderProgram::~D3D11ShaderProgram()
{
	SAFE_RELEASE(m_pVertexBlob);

	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pHullShader);
	SAFE_RELEASE(m_pDomainShader);
	SAFE_RELEASE(m_pGeometryShader);
	SAFE_RELEASE(m_pComputeShader);
}

bool D3D11ShaderProgram::CompileVertexShader(const char8* shader, const char8* enter)
{
	m_vertShaderText = shader;
	m_vertShaderEnter = enter;

	ID3D10Blob* pErrorMsg = nullptr;

	HRESULT hr = D3DCompile(shader, strlen(shader), nullptr, nullptr, nullptr, enter, "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &m_pVertexBlob, &pErrorMsg);
	if (pErrorMsg != nullptr)
	{
		m_strError = (LPCSTR)pErrorMsg->GetBufferPointer();
		TRACE("/////////////////Vertex Shader Compiler Error!/////////////////\n");
		TRACE(m_strError.c_str());
		TRACE("///////////////////////////////////////////////////////////////\n");
	}

	if (FAILED(hr))
	{
		SAFE_RELEASE(pErrorMsg);
		SAFE_RELEASE(m_pVertexBlob);
		return false;
	}

	hr = m_pDevice->CreateVertexShader(m_pVertexBlob->GetBufferPointer(), m_pVertexBlob->GetBufferSize(), nullptr, &m_pVertexShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pErrorMsg);
		SAFE_RELEASE(m_pVertexBlob);
		return false;
	}

	SAFE_RELEASE(pErrorMsg);

	return true;
}

bool D3D11ShaderProgram::CompilePixelShader(const char8* shader, const char8* enter)
{
	m_pixelShaderText = shader;
	m_pixelShaderEnter = enter;

	ID3D10Blob* pErrorMsg = nullptr;
	ID3D10Blob* pPSBlob = nullptr;

	HRESULT hr = D3DCompile(shader, strlen(shader), nullptr, nullptr, nullptr, enter, "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pPSBlob, &pErrorMsg);
	if (pErrorMsg != nullptr)
	{
		m_strError = (LPCSTR)pErrorMsg->GetBufferPointer();
		TRACE("/////////////////Pixel Shader Compiler Error!/////////////////\n");
		TRACE(m_strError.c_str());
		TRACE("//////////////////////////////////////////////////////////////\n");
	}

	if (FAILED(hr))
	{
		SAFE_RELEASE(pErrorMsg);
		SAFE_RELEASE(pPSBlob);
		return false;
	}

	hr = m_pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pErrorMsg);
		SAFE_RELEASE(pPSBlob);
		return false;
	}

	SAFE_RELEASE(pErrorMsg);
	SAFE_RELEASE(pPSBlob);
	return true;
}

void D3D11ShaderProgram::ApplyToDevice()
{
	if (m_pVertexShader != nullptr) m_pContext->VSSetShader(m_pVertexShader, nullptr, 0);
	if (m_pPixelShader != nullptr) m_pContext->PSSetShader(m_pPixelShader, nullptr, 0);
	if (m_pHullShader != nullptr) m_pContext->HSSetShader(m_pHullShader, nullptr, 0);
	if (m_pDomainShader != nullptr) m_pContext->DSSetShader(m_pDomainShader, nullptr, 0);
	if (m_pGeometryShader != nullptr) m_pContext->GSSetShader(m_pGeometryShader, nullptr, 0);
	if (m_pComputeShader != nullptr) m_pContext->CSSetShader(m_pComputeShader, nullptr, 0);
}