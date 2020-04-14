﻿#include "PreCompile.h"
#include "Pharos.h"

D3D11RenderTechnique::D3D11RenderTechnique()
{
	m_renderer = nullptr;
}

D3D11RenderTechnique::~D3D11RenderTechnique(void)
{
	for (auto iter : m_varMap)
	{
		SAFE_DELETE(iter.second);
	}

	for (VarBlock& block : m_blockList)
	{
		SAFE_DELETE(block.shaderData);
	}

	for (RenderPass* pass : m_passList)
	{
		SAFE_DELETE(pass);
	}
}

bool D3D11RenderTechnique::Create(RenderTechInfo* techInfo)
{
	m_renderer = static_cast<D3D11Renderer*>(sRenderMgr->GetCurrentRenderer());
	D3D11RenderTechInfo* d3dTechInfo = static_cast<D3D11RenderTechInfo*>(techInfo);

	m_techName = d3dTechInfo->GetTechniqueName();
	string shaderText = d3dTechInfo->GetTechniqueText();

	for (uint32 i = 0; i < d3dTechInfo->GetConstantBufferNum(); i++)
	{
		VarBlock  varBlock;
		varBlock.slot = d3dTechInfo->GetConstantBufferSlot(i);

		if (d3dTechInfo->GetShaderDataValid(varBlock.slot))
		{
			varBlock.shaderData = m_renderer->CreateShaderData();

			for (uint32 j = 0; j < d3dTechInfo->GetConstantBufferVariableNum(i); j++)
			{
				string varName = d3dTechInfo->GetConstantBufferVariableName(i, j);
				RenderVariable* var = new RenderVariable(varName.c_str(), 0xFF);
				m_varMap[varName] = var;
				varBlock.varList.push_back(var);
			}

			m_blockList.push_back(varBlock);
		}
	}

	for (uint32 i = 0; i < d3dTechInfo->GetVariableNum(); i++)
	{
		string varName = d3dTechInfo->GetVariableName(i);
		uint32 slot = d3dTechInfo->GetVariableSlot(i);

		if (d3dTechInfo->GetVariableValid(slot))
		{
			RenderVariable* var = new RenderVariable(varName.c_str(), slot);
			m_varMap[varName] = var;
			m_varList.push_back(var);
		}
	}

	uint32 passNum = d3dTechInfo->GetPassNum();
	m_passList.resize(passNum);
	
	for (uint32 i = 0; i < passNum; i++)
	{
		if (d3dTechInfo->GetPassValid(i))
		{
			string vertEnter = d3dTechInfo->GetPassVertexEnter(i);
			string pixelEnter = d3dTechInfo->GetPassPixelEnter(i);

			D3D11RenderPass* pass = new D3D11RenderPass();
			if(!pass->CreateShaderProgram(vertEnter.c_str(), pixelEnter.c_str(), shaderText.c_str()))
			{
				SAFE_DELETE(pass);
				continue;
			}

			const RasterizerStateDesc& rasterDesc = d3dTechInfo->GetPassRasterizerState(i);
			const BlendStateDesc& blendDesc = d3dTechInfo->GetPassBlendState(i);
			const DepthStencilStateDesc& depthDesc = d3dTechInfo->GetPassDepthStencilState(i);

			D3D11DepthStencilState* depthState = static_cast<D3D11DepthStencilState*>(m_renderer->CreateDepthStencilState(depthDesc));
			D3D11RasterizerState* rasterizerState = static_cast<D3D11RasterizerState*>(m_renderer->CreateRasterizerState(rasterDesc));
			D3D11BlendState* blendState = static_cast<D3D11BlendState*>(m_renderer->CreateBlendState(blendDesc));

			pass->BindDepthStencilState(depthState);
			pass->BindBlendState(blendState);
			pass->BindRasterizerState(rasterizerState);

			m_passList[i] = pass;
		}
	}

	return true;
}

RenderTechnique* D3D11RenderTechnique::Clone()
{
	D3D11RenderTechnique* tech = new D3D11RenderTechnique();
	tech->m_renderer = m_renderer;
	tech->m_techName = m_techName;

	for (VarBlock& varBlock : m_blockList)
	{
		VarBlock newVarBlock;
		newVarBlock.slot = varBlock.slot;
		newVarBlock.shaderData = m_renderer->CreateShaderData();

		for (RenderVariable* var : varBlock.varList)
		{
			string varName = var->GetName();
			uint32 slot = var->GetSlot();
			
			RenderVariable* new_var = new RenderVariable(varName.c_str(), slot);
			
			newVarBlock.varList.push_back(new_var);
			tech->m_varMap[varName] = new_var;
		}
		
		tech->m_blockList.push_back(newVarBlock);
	}

	for (RenderVariable* var : m_varList)
	{
		string varName = var->GetName();
		uint32 slot = var->GetSlot();
		
		RenderVariable* new_var = new RenderVariable(varName.c_str(), slot);
		
		tech->m_varList.push_back(new_var);
		tech->m_varMap[varName] = new_var;
	}

	for (uint32 i = 0; i < m_passList.size(); i++)
	{
		D3D11RenderPass* pass = static_cast<D3D11RenderPass*>(m_passList[i]);

		tech->m_passList.push_back(pass->Clone());
	}
	
	return tech;
}

void D3D11RenderTechnique::ApplyToDevice()
{
	for (uint32 i = 0; i < m_blockList.size(); i++)
	{
		VarBlock& varBlock = m_blockList[i];

		uint32 dataOffset = 0;

		for (uint32 j = 0; j < varBlock.varList.size(); j++)
		{
			RenderVariable* var = varBlock.varList[j];

			if (var == nullptr)
			{
				assert(false);
			}

			uint32 varDataSize = var->GetDataSize();

			if (var->IsDataChange())
			{
				MemoryBuffer& membuf = var->GetMemoryData();
				varBlock.shaderData->CopyData(membuf, dataOffset);
				var->SetDataChange(false);
			}

			dataOffset += varDataSize;
		}

		m_renderer->BindShaderData(varBlock.slot, varBlock.shaderData);
	}

	//绑定texture
	for (uint32 i = 0; i < m_varList.size(); i++)
	{
		RenderVariable* var = m_varList[i];
		uint32 slot = var->GetSlot();
		RenderTexture* tex = var->GetTexture();
		if (tex != nullptr) m_renderer->BindTexture(slot, tex);
	}
}