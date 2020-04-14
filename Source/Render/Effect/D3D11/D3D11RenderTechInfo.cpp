#include "PreCompile.h"
#include "Pharos.h"

D3D11RenderTechInfo::D3D11RenderTechInfo()
{
}

D3D11RenderTechInfo::~D3D11RenderTechInfo()
{
}

void D3D11RenderTechInfo::SetTechniqueName(const char8* name)
{
	m_techName = name;
}

const char8* D3D11RenderTechInfo::GetTechniqueName()
{
	return m_techName.c_str();
}

void D3D11RenderTechInfo::SetTechniqueText(const char8* text)
{
	m_techText = text;
}

const char8* D3D11RenderTechInfo::GetTechniqueText()
{
	return m_techText.c_str();
}

void D3D11RenderTechInfo::AddVariable(const char8* name, uint32 bind)
{
	m_varInfos.push_back({ true, name, bind });
}

uint32 D3D11RenderTechInfo::GetVariableNum()
{
	return (uint32)m_varInfos.size();
}

const char8* D3D11RenderTechInfo::GetVariableName(uint32 index)
{
	if (index >= m_varInfos.size()) return "";
	return m_varInfos[index].name.c_str();
}

uint32 D3D11RenderTechInfo::GetVariableSlot(uint32 index)
{
	if (index >= m_varInfos.size()) return 0xFFFFFFFF;
	return m_varInfos[index].bind;
}

void D3D11RenderTechInfo::SetVariableValid(uint32 bind, bool valid)
{
	for (uint32 i = 0; i < m_varInfos.size(); i++)
	{
		if (m_varInfos[i].bind == bind)
		{
			m_varInfos[i].valid = valid;
			return;
		}
	}
}

bool D3D11RenderTechInfo::GetVariableValid(uint32 bind)
{
	for (uint32 i = 0; i < m_varInfos.size(); i++)
	{
		if (m_varInfos[i].bind == bind)
		{
			return m_varInfos[i].valid;
		}
	}

	return false;
}

uint32 D3D11RenderTechInfo::AddConstantBuffer(const char8* name, uint32 bind)
{
	uint32 constantBufferSize = (uint32)m_constantBufInfos.size();
	m_constantBufInfos.push_back({ true, name, bind });
	return constantBufferSize;
}

void D3D11RenderTechInfo::AddConstantBufferVariable(uint32 index, const char8* varName)
{
	if (index >= m_constantBufInfos.size()) return;
	m_constantBufInfos[index].varList.push_back(varName);
}

uint32 D3D11RenderTechInfo::GetConstantBufferNum()
{
	return (uint32)m_constantBufInfos.size();
}

const char8* D3D11RenderTechInfo::GetConstantBufferName(uint32 index)
{
	if (index >= m_constantBufInfos.size()) return "";
	return m_constantBufInfos[index].name.c_str();
}

uint32 D3D11RenderTechInfo::GetConstantBufferSlot(uint32 index)
{
	if (index >= m_constantBufInfos.size()) return 0xFFFFFFFF;
	return m_constantBufInfos[index].bind;
}

uint32 D3D11RenderTechInfo::GetConstantBufferVariableNum(uint32 index)
{
	if (index >= m_constantBufInfos.size()) return 0xFFFFFFFF;
	return (uint32)m_constantBufInfos[index].varList.size();
}

const char8* D3D11RenderTechInfo::GetConstantBufferVariableName(uint32 index, uint32 varIndex)
{
	if (index >= m_constantBufInfos.size()) return "";
	if (varIndex >= m_constantBufInfos[index].varList.size()) return "";
	return m_constantBufInfos[index].varList[varIndex].c_str();
}

void D3D11RenderTechInfo::SetShaderDataValid(uint32 bind, bool valid)
{
	for (uint32 i = 0; i < m_constantBufInfos.size(); i++)
	{
		if (m_constantBufInfos[i].bind == bind)
		{
			m_constantBufInfos[i].valid = valid;
			return;
		}
	}
}

bool D3D11RenderTechInfo::GetShaderDataValid(uint32 bind)
{
	for (uint32 i = 0; i < m_constantBufInfos.size(); i++)
	{
		if (m_constantBufInfos[i].bind == bind)
		{
			return m_constantBufInfos[i].valid;
		}
	}

	return false;
}

uint32 D3D11RenderTechInfo::AddPass(const char8* vertEnter, const char8* pixelEnter)
{
	uint32 passInfoSize = (uint32)m_passInfos.size();
	m_passInfos.push_back({ true, vertEnter, pixelEnter });
	return passInfoSize;
}

void D3D11RenderTechInfo::SetPassState(uint32 index, const RasterizerStateDesc& rasterDesc)
{
	if (index >= m_passInfos.size()) return;
	m_passInfos[index].rasterDesc = rasterDesc;
}

void D3D11RenderTechInfo::SetPassState(uint32 index, const BlendStateDesc& blendDesc)
{
	if (index >= m_passInfos.size()) return;
	m_passInfos[index].blendDesc = blendDesc;
}

void D3D11RenderTechInfo::SetPassState(uint32 index, const DepthStencilStateDesc& depthDesc)
{
	if (index >= m_passInfos.size()) return;
	m_passInfos[index].depthDesc = depthDesc;
}

uint32 D3D11RenderTechInfo::GetPassNum()
{
	return (uint32)m_passInfos.size();
}

const char8* D3D11RenderTechInfo::GetPassVertexEnter(uint32 index)
{
	if (index >= m_passInfos.size()) return "";
	return m_passInfos[index].vertEnter.c_str();
}

const char8* D3D11RenderTechInfo::GetPassPixelEnter(uint32 index)
{
	if (index >= m_passInfos.size()) return "";
	return m_passInfos[index].pixelEnter.c_str();
}

const RasterizerStateDesc& D3D11RenderTechInfo::GetPassRasterizerState(uint32 index)
{
	return m_passInfos[index].rasterDesc;
}

const BlendStateDesc& D3D11RenderTechInfo::GetPassBlendState(uint32 index)
{
	return m_passInfos[index].blendDesc;
}

const DepthStencilStateDesc& D3D11RenderTechInfo::GetPassDepthStencilState(uint32 index)
{
	return m_passInfos[index].depthDesc;
}

void D3D11RenderTechInfo::SetPassValid(uint32 bind, bool valid)
{
	if (bind >= m_passInfos.size()) return;
	m_passInfos[bind].valid = valid;
}

bool D3D11RenderTechInfo::GetPassValid(uint32 bind)
{
	if (bind >= m_passInfos.size()) return false;
	return m_passInfos[bind].valid;
}