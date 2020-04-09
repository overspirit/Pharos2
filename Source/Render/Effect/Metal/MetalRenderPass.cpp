#include "PreCompile.h"
#include "Pharos.h"

MetalRenderPass::MetalRenderPass()
{
	m_renderer = nullptr;

	m_shader = nullptr;

	m_blendState = nullptr;
	m_depthState = nullptr;
	m_rasterizerState = nullptr;

	m_frameBuf = nullptr;
}

MetalRenderPass::~MetalRenderPass()
{
	SAFE_DELETE(m_shader);

	SAFE_DELETE(m_blendState);
	SAFE_DELETE(m_depthState);
	SAFE_DELETE(m_rasterizerState);

	SAFE_DELETE(m_frameBuf);
}

bool MetalRenderPass::CreateShaderProgram(const char8* vertEnter, const char8* pixelEnter, const char8* shaderText)
{
	m_renderer = static_cast<MetalRenderer*>(sRenderMgr->GetCurrentRenderer());

	MetalShaderProgram* shader = static_cast<MetalShaderProgram*>(m_renderer->GenerateRenderProgram());
	if (!shader->CompileVertexShader(shaderText, vertEnter) || !shader->CompilePixelShader(shaderText, pixelEnter))
	{
		SAFE_DELETE(shader);
		return false;
	}

	m_shader = shader;

	return true;
}

MetalRenderPass* MetalRenderPass::Clone()
{
	MetalRenderPass* pass = new MetalRenderPass();

	pass->m_renderer = m_renderer;

	pass->m_shader = static_cast<MetalShaderProgram*>(m_shader->Clone());

	if (m_blendState != nullptr) pass->m_blendState = static_cast<MetalBlendState*>(m_blendState->Clone());
	if (m_depthState != nullptr) pass->m_depthState = static_cast<MetalDepthStencilState*>(m_depthState->Clone());
	if (m_rasterizerState != nullptr) pass->m_rasterizerState = static_cast<MetalRasterizerState*>(m_rasterizerState->Clone());

	if (m_frameBuf != nullptr)
	{
		int32 width = m_frameBuf->GetWidth();
		int32 height = m_frameBuf->GetHeight();
		RenderFrameBuffer* frameBuf = m_renderer->CreateFrameBuffer(width, height);
		pass->m_frameBuf = static_cast<MetalFrameBuffer*>(frameBuf);
	}

	return pass;
}

void MetalRenderPass::BindShaderProgram(RenderProgram* shader)
{
	m_shader = static_cast<MetalShaderProgram*>(shader);
}

void MetalRenderPass::BindFrameBuffer(RenderFrameBuffer* frameBuf)
{
	m_frameBuf = static_cast<MetalFrameBuffer*>(frameBuf);
}

void MetalRenderPass::BindRasterizerState(RenderRasterizerState* state)
{
	m_rasterizerState = static_cast<MetalRasterizerState*>(state);
}

void MetalRenderPass::BindDepthStencilState(RenderDepthStencilState* state)
{
	m_depthState = static_cast<MetalDepthStencilState*>(state);
}

void MetalRenderPass::BindBlendState(RenderBlendState* state)
{
	m_blendState = static_cast<MetalBlendState*>(state);
}

void MetalRenderPass::ApplyToDevice()
{
	if (m_frameBuf != nullptr)
	{
		m_renderer->BindFrameBuffer(m_frameBuf);
	}

	m_renderer->BindDepthStencilState(m_depthState);
	m_renderer->BindRasterizerState(m_rasterizerState);
	m_renderer->BindBlendState(m_blendState);

	m_renderer->BindProgram(m_shader);
}
