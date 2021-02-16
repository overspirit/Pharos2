#include "PreCompile.h"
#include "Pharos.h"

Material::Material(const char8* materialName)
{
    m_materialName = materialName;
    
    m_renderPipeline = nullptr;
    m_renderSet = nullptr;

    m_viewVar = nullptr;
    m_projVar = nullptr;
    m_worldVar = nullptr;
    m_boneVar = nullptr;
    m_lightColorVar = nullptr;
    m_environColorVar = nullptr;
    m_lightDirVar = nullptr;
    m_colorTextureVar = nullptr;
}

Material::~Material()
{
	SAFE_DELETE(m_renderPipeline);
	SAFE_DELETE(m_renderSet);

    SAFE_DELETE(m_viewVar);
    SAFE_DELETE(m_projVar);
    SAFE_DELETE(m_worldVar);
    SAFE_DELETE(m_boneVar);
    SAFE_DELETE(m_lightColorVar);
    SAFE_DELETE(m_environColorVar);
    SAFE_DELETE(m_lightDirVar);
    SAFE_DELETE(m_colorTextureVar);

	for (UniformSet& uniformSet : m_uniformSets)
	{
		SAFE_DELETE(uniformSet.uniformBuf);
		SAFE_DELETE(uniformSet.memberBuf);
	}
}

void Material::SetViewParamValue(const Matrix4& viewMat)
{
    if (m_viewVar != nullptr)
    {
        m_viewVar->SetValue(viewMat);
    }
}

void Material::SetProjParamValue(const Matrix4& projMat)
{
    if (m_projVar != nullptr)
    {
        m_projVar->SetValue(projMat);
    }
}

void Material::SetWorldParamValue(const Matrix4& worldMat)
{
    if (m_worldVar != nullptr)
    {
        m_worldVar->SetValue(worldMat);
    }
}

void Material::SetBoneParamValue(const Matrix4* boneMat, uint32 boneSize)
{
    if (m_boneVar != nullptr)
    {
        m_boneVar->SetValue(boneMat, boneSize);
    }
}

void Material::SetLightDirectionParamValue(const Vector3Df& lightDir)
{
    if (m_lightDirVar != nullptr)
    {        
        m_lightDirVar->SetValue(lightDir);
    }
}

void Material::SetEnvironmentColorParamValue(Color4 envColor)
{
    if (m_environColorVar != nullptr)
    {
        m_environColorVar->SetValue(Color4f(envColor));
    }
}

void Material::SetLightColorParamValue(Color4 lightColor)
{
    if (m_lightColorVar != nullptr)
    {
        m_lightColorVar->SetValue(Color4f(lightColor));
    }
}

void Material::SetTextureParamValue(const char8* valueName, RenderTexture* texture)
{
    if (strcmp(valueName, "g_tex") == 0)
    {
        if (m_colorTextureVar != nullptr)
        {
            m_colorTextureVar->SetValue(texture);
        }
    }
    else
    {
        SAFE_DELETE(texture);
    }
    
}

void Material::SetShaderProgram(RenderProgram* renderProgram)
{
    m_renderPipeline = sRenderer->GenerateRenderPipeline();
	m_renderPipeline->SetProgramShader(renderProgram);
	
    m_renderSet = sRenderer->GenerateRenderResuourceSet();
}

void Material::SetTransparentEnabled(bool enabled)
{
    m_blendState.blendEnable = enabled;
    m_blendState.alphaToCoverageEnable = false;
    m_blendState.srcBlend = BLEND_SRC_ALPHA;
    m_blendState.destBlend = BLEND_INV_SRC_ALPHA;
    m_blendState.blendOp = BLEND_OP_ADD;
    m_blendState.srcBlendAlpha = BLEND_SRC_ALPHA;
    m_blendState.destBlendAlpha = BLEND_INV_SRC_ALPHA;
    m_blendState.blendOpAlpha = BLEND_OP_ADD;
    
    m_renderPipeline->SetBlendState(m_blendState);
}

void Material::SetForceDepthWrite(bool force)
{
    m_depthState.depthEnable = true;
    m_depthState.depthWriteMask = DEPTH_WRITE_ALL;
    m_depthState.depthFunc = force ? COMPARISON_ALWAYS : COMPARISON_LESS;
    m_depthState.stencilEnable = false;
    m_depthState.stencilRefValue = 0;
    m_depthState.stencilWriteMask = 0xFF;
    m_depthState.frontStencilFailOp = STENCIL_OP_KEEP;
    m_depthState.frontStencilDepthFailOp = STENCIL_OP_KEEP;
    m_depthState.frontStencilPassOp = STENCIL_OP_KEEP;
    m_depthState.frontStencilFunc = COMPARISON_ALWAYS;
    m_depthState.backStencilFailOp = STENCIL_OP_KEEP;
    m_depthState.backStencilDepthFailOp = STENCIL_OP_KEEP;
    m_depthState.backStencilPassOp = STENCIL_OP_KEEP;
    m_depthState.backStencilFunc = COMPARISON_ALWAYS;
    
    m_renderPipeline->SetDepthStencilState(m_depthState);
}

void Material::SetCullBackFace(bool cull)
{
    m_rasterizerState.fillMode = FILL_SOLID;
    m_rasterizerState.cullMode = cull ? CULL_BACK : CULL_NONE;
    //m_rasterizerState.frontCounterClockwise = false;
    m_rasterizerState.depthBias = 0;
    m_rasterizerState.depthBiasClamp = 0;
    m_rasterizerState.slopeScaledDepthBias = 0;
    m_rasterizerState.depthClipEnable = true;
    m_rasterizerState.scissorEnable = false;
    m_rasterizerState.multisampleEnable = false;
    m_rasterizerState.antialiasedLineEnable = false;
    
    m_renderPipeline->SetRasterizerState(m_rasterizerState);
}

void Material::SetClockwiseFrontFace(bool clockwise)
{
    m_rasterizerState.fillMode = FILL_SOLID;
    //m_rasterizerState.cullMode = CULL_BACK;
    m_rasterizerState.frontCounterClockwise = !clockwise;
    m_rasterizerState.depthBias = 0;
    m_rasterizerState.depthBiasClamp = 0;
    m_rasterizerState.slopeScaledDepthBias = 0;
    m_rasterizerState.depthClipEnable = true;
    m_rasterizerState.scissorEnable = false;
    m_rasterizerState.multisampleEnable = false;
    m_rasterizerState.antialiasedLineEnable = false;
    
    m_renderPipeline->SetRasterizerState(m_rasterizerState);
}

void Material::SetTextureVariable(const char8* name, uint32 slot)
{
    if (strcmp(name, "g_tex") == 0)
    {
        m_colorTextureVar = new RenderVariable(slot, m_renderSet);
    }
}

void Material::UpdateParamValue()
{
    for (uint32 i = 0; i < m_uniformSets.size(); i++)
    {
		auto& uniformSet = m_uniformSets[i];

		if (uniformSet.memberBuf != nullptr && uniformSet.uniformBuf != nullptr)
		{
			uniformSet.uniformBuf->CopyData(*uniformSet.memberBuf);

			m_renderSet->SetVertexUniformBuffer(i, uniformSet.uniformBuf);
		}
    }
}

uint32 Material::AddUniformaVariable(const char8* name, uint32 size, uint32 slot)
{
	uint32 setIndex = m_uniformSets.size();	
    m_uniformSets.resize(slot + 1);

    RenderBuffer* uniformBuf = sRenderer->GenerateRenderBuffer(UNIFORM_BUFFFER);
    uniformBuf->Allocate(size);

    UniformSet uniformSet;
    uniformSet.uniformBuf = uniformBuf;
    uniformSet.memberBuf = new MemoryBuffer(size);
    uniformSet.useSize = 0;
    m_uniformSets[slot] = uniformSet;

    return setIndex;
}

void Material::SetUniformMember(uint32 varIndex, const char8* name, uint32 size)
{
    UniformSet& uniformSet = m_uniformSets[varIndex];

    if (strcmp(name, "g_view") == 0)
    {
        m_viewVar = new RenderVariable(uniformSet.memberBuf, uniformSet.useSize);
    }
    else if (strcmp(name, "g_proj") == 0)
    {
        m_projVar = new RenderVariable(uniformSet.memberBuf, uniformSet.useSize);
    }
    else if (strcmp(name, "g_world") == 0)
    {
        m_worldVar = new RenderVariable(uniformSet.memberBuf, uniformSet.useSize);
    }
    else if (strcmp(name, "g_boneMat") == 0)
    {
        m_boneVar = new RenderVariable(uniformSet.memberBuf, uniformSet.useSize);
    }
    else if (strcmp(name, "g_light_color") == 0)
    {
        m_lightColorVar = new RenderVariable(uniformSet.memberBuf, uniformSet.useSize);
    }
    else if (strcmp(name, "g_environment_color") == 0)
    {
        m_environColorVar = new RenderVariable(uniformSet.memberBuf, uniformSet.useSize);
    }
    else if (strcmp(name, "g_light_direction") == 0)
    {
        m_lightDirVar = new RenderVariable(uniformSet.memberBuf, uniformSet.useSize);
    }

    uniformSet.useSize += size;
}
