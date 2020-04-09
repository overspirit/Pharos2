#include "PreCompile.h"
#include "Pharos.h"

MetalRenderTechInfo::MetalRenderTechInfo()
{
}

MetalRenderTechInfo::~MetalRenderTechInfo()
{
}

void MetalRenderTechInfo::SetTechniqueName(const char8* name)
{
	m_techName = name;
}

const char8* MetalRenderTechInfo::GetTechniqueName()
{
	return m_techName.c_str();
}

void MetalRenderTechInfo::SetTechniqueText(const char8* text)
{
	m_techText = text;
}

const char8* MetalRenderTechInfo::GetTechniqueText()
{
	return m_techText.c_str();
}

void MetalRenderTechInfo::AddVariable(const char8* name, uint32 bind)
{
	m_varInfos.push_back({ true, name, bind });
}

uint32 MetalRenderTechInfo::GetVariableNum()
{
	return (uint32)m_varInfos.size();
}

const char8* MetalRenderTechInfo::GetVariableName(uint32 index)
{
	if (index >= m_varInfos.size()) return "";
	return m_varInfos[index].name.c_str();
}

uint32 MetalRenderTechInfo::GetVariableSlot(uint32 index)
{
	if (index >= m_varInfos.size()) return 0xFFFFFFFF;
	return m_varInfos[index].bind;
}

void MetalRenderTechInfo::SetVariableValid(uint32 bind, bool valid)
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

bool MetalRenderTechInfo::GetVariableValid(uint32 bind)
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

uint32 MetalRenderTechInfo::AddConstantBuffer(const char8* name, uint32 bind)
{
	uint32 constantBufferSize = (uint32)m_constantBufInfos.size();
	m_constantBufInfos.push_back({ true, name, bind });
	return constantBufferSize;
}

void MetalRenderTechInfo::AddConstantBufferVariable(uint32 index, const char8* varName)
{
	if (index >= m_constantBufInfos.size()) return;
	m_constantBufInfos[index].varList.push_back(varName);
}

uint32 MetalRenderTechInfo::GetConstantBufferNum()
{
	return (uint32)m_constantBufInfos.size();
}

const char8* MetalRenderTechInfo::GetConstantBufferName(uint32 index)
{
	if (index >= m_constantBufInfos.size()) return "";
	return m_constantBufInfos[index].name.c_str();
}

uint32 MetalRenderTechInfo::GetConstantBufferSlot(uint32 index)
{
	if (index >= m_constantBufInfos.size()) return 0xFFFFFFFF;
	return m_constantBufInfos[index].bind;
}

uint32 MetalRenderTechInfo::GetConstantBufferVariableNum(uint32 index)
{
	if (index >= m_constantBufInfos.size()) return 0xFFFFFFFF;
	return (uint32)m_constantBufInfos[index].varList.size();
}

const char8* MetalRenderTechInfo::GetConstantBufferVariableName(uint32 index, uint32 varIndex)
{
	if (index >= m_constantBufInfos.size()) return "";
	if (varIndex >= m_constantBufInfos[index].varList.size()) return "";
	return m_constantBufInfos[index].varList[varIndex].c_str();
}

void MetalRenderTechInfo::SetShaderDataValid(uint32 bind, bool valid)
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

bool MetalRenderTechInfo::GetShaderDataValid(uint32 bind)
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

uint32 MetalRenderTechInfo::AddPass(const char8* vertEnter, const char8* pixelEnter)
{
	uint32 passInfoSize = (uint32)m_passInfos.size();
	m_passInfos.push_back({ true, vertEnter, pixelEnter });
	return passInfoSize;
}

void MetalRenderTechInfo::SetPassState(uint32 index, const RasterizerStateDesc& rasterDesc)
{
	if (index >= m_passInfos.size()) return;
	m_passInfos[index].rasterDesc = rasterDesc;
}

void MetalRenderTechInfo::SetPassState(uint32 index, const BlendStateDesc& blendDesc)
{
	if (index >= m_passInfos.size()) return;
	m_passInfos[index].blendDesc = blendDesc;
}

void MetalRenderTechInfo::SetPassState(uint32 index, const DepthStencilStateDesc& depthDesc)
{
	if (index >= m_passInfos.size()) return;
	m_passInfos[index].depthDesc = depthDesc;
}

uint32 MetalRenderTechInfo::GetPassNum()
{
	return (uint32)m_passInfos.size();
}

const char8* MetalRenderTechInfo::GetPassVertexEnter(uint32 index)
{
	if (index >= m_passInfos.size()) return "";
	return m_passInfos[index].vertEnter.c_str();
}

const char8* MetalRenderTechInfo::GetPassPixelEnter(uint32 index)
{
	if (index >= m_passInfos.size()) return "";
	return m_passInfos[index].pixelEnter.c_str();
}

const RasterizerStateDesc& MetalRenderTechInfo::GetPassRasterizerState(uint32 index)
{
	return m_passInfos[index].rasterDesc;
}

const BlendStateDesc& MetalRenderTechInfo::GetPassBlendState(uint32 index)
{
	return m_passInfos[index].blendDesc;
}

const DepthStencilStateDesc& MetalRenderTechInfo::GetPassDepthStencilState(uint32 index)
{
	return m_passInfos[index].depthDesc;
}

void MetalRenderTechInfo::SetPassValid(uint32 bind, bool valid)
{
	if (bind >= m_passInfos.size()) return;
	m_passInfos[bind].valid = valid;
}

bool MetalRenderTechInfo::GetPassValid(uint32 bind)
{
	if (bind >= m_passInfos.size()) return false;
	return m_passInfos[bind].valid;
}
