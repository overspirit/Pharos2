#include "PreCompile.h"
#include "RenderGlobal.h"

OpenGLRenderTechnique::OpenGLRenderTechnique()
{

}

OpenGLRenderTechnique::~OpenGLRenderTechnique()
{

}

bool OpenGLRenderTechnique::Create(const TechniqueInfo& info)
{
	m_techName = info.techName;
    
    PassBindInfo    bindInfo;
    bindInfo.blockInfos = info.blockInfos;
    bindInfo.uniformInfos = info.uniformInfos;
    
	for (uint32 i = 0; i < info.attrInfos.size(); i++)
	{
		string name = info.attrInfos[i].name;
		string sema = info.attrInfos[i].sema;
		uint32 bind = GetSemanticIndex(sema.c_str());
		bindInfo.attrInfos.push_back({ name, bind });
	}

	for (uint32 i = 0; i < info.passInfos.size(); i++)
	{
		OpenGLRenderPassPtr pass = MakeSharedPtr<OpenGLRenderPass>();
		if (!pass->Create(info.passInfos[i], bindInfo))
		{
            pass = nullptr;

			//pass 建立错误.....
			assert(false);
		}

		m_passList.push_back(pass);
	}

    //build uniform variable list
	for (uint32 i = 0; i < info.uniformInfos.size(); i++)
	{
		string name = info.uniformInfos[i].name;
		uint32 slot = info.uniformInfos[i].bind;
		RenderVariablePtr var = MakeSharedPtr<RenderVariable>(name.c_str(), slot);
		m_varDataMap[name] = var;
	}

    //build unfirom block list
	for (uint32 i = 0; i < info.blockInfos.size(); i++)
	{
		OpenGLUniformBufPtr varBuf = MakeSharedPtr<OpenGLUniformBuf>();
		if (varBuf->Create(info.blockInfos[i]))
		{
			m_varBufList.push_back(varBuf);
		}
	}

	return true;
}

RenderTechniquePtr OpenGLRenderTechnique::Clone()
{
	OpenGLRenderTechniquePtr tech = MakeSharedPtr<OpenGLRenderTechnique>();

	tech->m_techName = m_techName;

	for (auto iter : m_varDataMap)
	{
		string name = iter.first;
		RenderVariablePtr var = iter.second;
		uint32 slot = var->GetSlot();
		RenderVariablePtr new_var = MakeSharedPtr<RenderVariable>(name.c_str(), slot);
		tech->m_varDataMap[name] = new_var;
	}

	for (auto varBuf : m_varBufList)
	{
		OpenGLUniformBufPtr uniformBuf = static_pointer_cast<OpenGLUniformBuf>(varBuf);
		tech->m_varBufList.push_back(uniformBuf->Clone());
	}

	for (uint32 i = 0; i < m_passList.size(); i++)
	{
		OpenGLRenderPassPtr oglPass = static_pointer_cast<OpenGLRenderPass>(m_passList[i]);
		tech->m_passList.push_back(oglPass->Clone());
	}

	return tech;
}

void OpenGLRenderTechnique::ApplyToDevice()
{
	for (auto iter : m_varDataMap)
	{
		RenderVariablePtr var = iter.second;
		if (var->GetVariableType() == Render::TYPE_TEXTURE)
		{
			uint32 slot = var->GetSlot();
			IRenderTexturePtr tex = var->GetTexture();
			OpenGLTexturePtr glTex = static_pointer_cast<OpenGLTexture>(tex);
			if (glTex != NULL)
			{
				glTex->ApplyToDevice(slot);
			}
		}
	}

	for (uint32 i = 0; i < m_varBufList.size(); i++)
	{
		OpenGLUniformBufPtr uniBuf = static_pointer_cast<OpenGLUniformBuf>(m_varBufList[i]);
		if (uniBuf != NULL)
		{
			uniBuf->ApplyToDevice();
		}
	}
}
