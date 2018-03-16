#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"

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

bool D3D11RenderTechnique::Create(const char8* effectText, const TechniqueInfo& info)
{
	m_renderer = static_cast<D3D11Renderer*>(sRenderMgr->GetCurrentRenderer());

	m_techName = info.techName;

	for (uint32 i = 0; i < info.constantBufInfos.size(); i++)
	{
		const ConstantBuffInfo& bufInfo = info.constantBufInfos[i];

		VarBlock  varBlock;
		varBlock.slot = bufInfo.bind;
		varBlock.shaderData = m_renderer->CreateShaderData();

		for (uint32 j = 0; j < bufInfo.varList.size(); j++)
		{
			string varName = bufInfo.varList[j];
			RenderVariable* var = new RenderVariable(varName.c_str(), 0xFF);
			m_varMap[varName] = var;
			varBlock.varList.push_back(var);
		}

		m_blockList.push_back(varBlock);
	}

	for (uint32 i = 0; i < info.varInfos.size(); i++)
	{
		string varName = info.varInfos[i].name;
		uint32 slot = info.varInfos[i].bind;

		RenderVariable* var = new RenderVariable(varName.c_str(), slot);
		m_varMap[varName] = var;
		m_varList.push_back(var);
	}

	for (uint32 i = 0; i < info.passInfos.size(); i++)
	{
		D3D11RenderPass* pass = new D3D11RenderPass();
		if (!pass->Create(effectText, info.passInfos[i]))
		{
			pass = nullptr;
		}

		m_passList.push_back(pass);
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
				varBlock.shaderData->CopyData(&membuf, dataOffset);
				var->SetDataChange(false);
			}

			dataOffset += varDataSize;
		}
	
		varBlock.shaderData->ApplyToDevice(varBlock.slot);
	}

	//绑定texture
	for (uint32 i = 0; i < m_varList.size(); i++)
	{
		RenderVariable* var = m_varList[i];
		//RENDER_VAR_TYPE varType = var->GetVariableType();
		//switch (varType)
		//{
			//case TYPE_CONCRETE: break;
			//case TYPE_TEXTURE:
			//{
				uint32 slot = var->GetSlot();
				RenderTexture* tex = var->GetTexture();
				if(tex != nullptr) m_renderer->BindTexture(slot, tex);
			//}
			//break;
		//}
	}
}