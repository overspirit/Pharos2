#include "PreCompile.h"
#include "Pharos.h"

RenderTexture* Material::s_unitTexture = nullptr;

Material::Material(const char8* materialName)
{
    m_materialName = materialName;
    
    m_renderPipeline = nullptr;
    m_renderSet = nullptr;

    m_sceneParamBuf = nullptr;
    m_modelParamBuf = nullptr;
    m_lightParamBuf = nullptr;
    m_materialParamBuf = nullptr;
    
    m_lightParam.lightDir = Vector4Df(0.577350259f, -0.577350259f, 0.577350259f, 1.0f);
    m_lightParam.lightColor = 0xFFFFFFFF;
    
    //m_materialParam.materialColor = 0xFF3F3F3F;
    m_materialParam.materialColor = 0xFFFFFFFF;
    m_materialParam.ambientColor = 0xFFFFFFFF;
    m_materialParam.diffuseColor = 0xFFFFFFFF;
    m_materialParam.specularColor = 0xFFFFFFFF;
    m_materialParam.albedoPow = Vector4Df(0.3f, 0.4f, 0.3f, 16.0f);
    
    m_colorTexture = nullptr;
    m_bumpTexture = nullptr;
}

Material::~Material()
{
	SAFE_DELETE(m_renderPipeline);
	SAFE_DELETE(m_renderSet);

    SAFE_DELETE(m_sceneParamBuf);
    SAFE_DELETE(m_modelParamBuf);
    SAFE_DELETE(m_lightParamBuf);
    SAFE_DELETE(m_materialParamBuf);
    
    SAFE_DELETE(s_unitTexture);
}

bool Material::InitWithShaderProgram(RenderProgram* renderProgram)
{
    if (s_unitTexture == nullptr)
    {
        s_unitTexture = sRenderer->CreateTexture2D(1, 1, EPF_RGBA8_UNORM);
        
        Color4 unitTextureData = 0xFFFFFFFF;
        s_unitTexture->CopyFromData(&unitTextureData, sizeof(Color4));
    }
    
    m_renderPipeline = sRenderer->GenerateRenderPipeline();
    m_renderPipeline->SetProgramShader(renderProgram);
    
    m_renderSet = sRenderer->GenerateRenderResuourceSet();
    
    m_sceneParamBuf = sRenderer->GenerateRenderBuffer(UNIFORM_BUFFFER);
    m_sceneParamBuf->Allocate(sizeof(PerScene));
    
    m_modelParamBuf = sRenderer->GenerateRenderBuffer(UNIFORM_BUFFFER);
    m_modelParamBuf->Allocate(sizeof(PerModel));
    
    m_lightParamBuf = sRenderer->GenerateRenderBuffer(UNIFORM_BUFFFER);
    m_lightParamBuf->Allocate(sizeof(SceneLight));
    
    m_materialParamBuf = sRenderer->GenerateRenderBuffer(UNIFORM_BUFFFER);
    m_materialParamBuf->Allocate(sizeof(PerMaterial));
    
    m_colorTexture = s_unitTexture;
    m_bumpTexture = s_unitTexture;
    
    return true;
}

void Material::SetViewParamValue(const Matrix4& viewMat, const Vector3Df& cameraPos)
{
    m_sceneParam.view = viewMat;
    m_sceneParam.camerPos = Vector4Df(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
}

void Material::SetProjParamValue(const Matrix4& projMat)
{
    m_sceneParam.proj = projMat;
}

void Material::SetWorldParamValue(const Matrix4& worldMat)
{
    m_modelParam.world = worldMat;
}

void Material::SetBoneParamValue(const Matrix4* boneMat, uint32 boneSize)
{
    uint32 copySize = Math::minimum(boneSize, 255u);
    for (uint i = 0; i < copySize; i++)
    {
        m_modelParam.bone[i] = boneMat[i];
    }
    
    for (uint32 i = 0; i < 255u - copySize; i++)
    {
        m_modelParam.bone[copySize + i].MakeIdentity();
    }
}

void Material::SetLightDirectionParamValue(const Vector3Df& lightDir)
{
    m_lightParam.lightDir = Vector4Df(lightDir.x, lightDir.y, lightDir.z, 0);
}

void Material::SetLightColorParamValue(Color4 lightColor)
{
    m_lightParam.lightColor = lightColor;
}

void Material::SetMaterialColorParamValue(Color4 color)
{
    m_materialParam.materialColor = color;
}

void Material::SetAbmbinetColorParamValue(Color4 color)
{
    m_materialParam.ambientColor = color;
}

void Material::SetDiffuseColorParamValue(Color4 color)
{
    m_materialParam.diffuseColor = color;
}

void Material::SetSpecularColorParamValue(Color4 color)
{
    m_materialParam.specularColor = color;
}

void Material::SetAbmbinetRatioParamValue(float32 ratio)
{
    m_materialParam.albedoPow.x = ratio;
}

void Material::SetDiffuseRatioParamValue(float32 ratio)
{
    m_materialParam.albedoPow.y = ratio;
}

void Material::SetSpecularRatioParamValue(float32 ratio)
{
    m_materialParam.albedoPow.z = ratio;
}

void Material::SetSpecularPowParamValue(float32 pow)
{
    m_materialParam.albedoPow.w = pow;
}

void Material::SetColorTextureParamValue(RenderTexture* texture)
{
    m_colorTexture = texture;
}

void Material::SetBumpTextureParamValue(RenderTexture* texture)
{
    m_bumpTexture = texture;
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

void Material::UpdateParamValue()
{    
    m_sceneParamBuf->CopyData(&m_sceneParam, sizeof(PerScene));
    m_modelParamBuf->CopyData(&m_modelParam, sizeof(PerModel));
    m_lightParamBuf->CopyData(&m_lightParam, sizeof(SceneLight));
    m_materialParamBuf->CopyData(&m_materialParam, sizeof(PerMaterial));
    
    m_renderSet->SetVertexUniformBuffer(0, m_sceneParamBuf);
    m_renderSet->SetVertexUniformBuffer(1, m_modelParamBuf);
    m_renderSet->SetVertexUniformBuffer(2, m_lightParamBuf);
    m_renderSet->SetVertexUniformBuffer(3, m_materialParamBuf);
    
    m_renderSet->SetFragmentUniformBuffer(0, m_sceneParamBuf);
    m_renderSet->SetFragmentUniformBuffer(1, m_modelParamBuf);
    m_renderSet->SetFragmentUniformBuffer(2, m_lightParamBuf);
    m_renderSet->SetFragmentUniformBuffer(3, m_materialParamBuf);
    m_renderSet->SetFragmentTexture(4, m_colorTexture);
    m_renderSet->SetFragmentTexture(5, m_bumpTexture);
}
