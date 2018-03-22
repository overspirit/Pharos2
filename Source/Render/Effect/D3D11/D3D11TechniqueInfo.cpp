#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

D3D11TechniqueInfo::D3D11TechniqueInfo()
{
}

D3D11TechniqueInfo::~D3D11TechniqueInfo()
{
}

void D3D11TechniqueInfo::SetTechniqueName(const char8* name)
{
	m_techName = name;
}

const char8* D3D11TechniqueInfo::GetTechniqueName()
{
	return m_techName.c_str();
}

void D3D11TechniqueInfo::SetTechniqueText(const char8* text)
{
	m_techText = text;
}

const char8* D3D11TechniqueInfo::GetTechniqueText()
{
	return m_techText.c_str();
}

void D3D11TechniqueInfo::AddVariable(const char8* name, uint32 bind)
{
	m_varInfos.push_back({ name, bind });
}

uint32 D3D11TechniqueInfo::GetVariableNum()
{
	return (uint32)m_varInfos.size();
}

const char8* D3D11TechniqueInfo::GetVariableName(uint32 index)
{
	return m_varInfos[index].name.c_str();
}

uint32 D3D11TechniqueInfo::GetVariableSlot(uint32 index)
{
	return m_varInfos[index].bind;
}

uint32 D3D11TechniqueInfo::AddConstantBuffer(const char8* name, uint32 bind)
{
	uint32 constantBufferSize = (uint32)m_constantBufInfos.size();
	m_constantBufInfos.push_back({ name, bind });
	return constantBufferSize;
}

void D3D11TechniqueInfo::AddConstantBufferVariable(uint32 index, const char8* varName)
{
	m_constantBufInfos[index].varList.push_back(varName);
}

uint32 D3D11TechniqueInfo::GetConstantBufferNum()
{
	return (uint32)m_constantBufInfos.size();
}

const char8* D3D11TechniqueInfo::GetConstantBufferName(uint32 index)
{
	return m_constantBufInfos[index].name.c_str();
}

uint32 D3D11TechniqueInfo::GetConstantBufferSlot(uint32 index)
{
	return m_constantBufInfos[index].bind;
}

uint32 D3D11TechniqueInfo::GetConstantBufferVariableNum(uint32 index)
{
	return (uint32)m_constantBufInfos[index].varList.size();
}

const char8* D3D11TechniqueInfo::GetConstantBufferVariableName(uint32 index, uint32 varIndex)
{
	return m_constantBufInfos[index].varList[varIndex].c_str();
}

uint32 D3D11TechniqueInfo::AddPass(const char8* vertEnter, const char8* pixelEnter)
{
	uint32 passInfoSize = (uint32)m_passInfos.size();
	m_passInfos.push_back({ vertEnter, pixelEnter });
	return passInfoSize;
}

void D3D11TechniqueInfo::SetPassState(uint32 index, const RasterizerStateDesc& rasterDesc)
{
	m_passInfos[index].rasterDesc = rasterDesc;
}

void D3D11TechniqueInfo::SetPassState(uint32 index, const BlendStateDesc& blendDesc)
{
	m_passInfos[index].blendDesc = blendDesc;
}

void D3D11TechniqueInfo::SetPassState(uint32 index, const DepthStencilStateDesc& depthDesc)
{
	m_passInfos[index].depthDesc = depthDesc;
}

uint32 D3D11TechniqueInfo::GetPassNum()
{
	return (uint32)m_passInfos.size();
}

const char8* D3D11TechniqueInfo::GetPassVertexEnter(uint32 index)
{
	return m_passInfos[index].vertEnter.c_str();
}

const char8* D3D11TechniqueInfo::GetPassPixelEnter(uint32 index)
{
	return m_passInfos[index].pixelEnter.c_str();
}

const RasterizerStateDesc& D3D11TechniqueInfo::GetPassRasterizerState(uint32 index)
{
	return m_passInfos[index].rasterDesc;
}

const BlendStateDesc& D3D11TechniqueInfo::GetPassBlendState(uint32 index)
{
	return m_passInfos[index].blendDesc;
}

const DepthStencilStateDesc& D3D11TechniqueInfo::GetPassDepthStencilState(uint32 index)
{
	return m_passInfos[index].depthDesc;
}