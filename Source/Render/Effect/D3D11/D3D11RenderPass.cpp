#include "PreCompile.h"
#include "Pharos.h"

D3D11RenderPass::D3D11RenderPass()
{
	m_renderer = nullptr;

	m_shader = nullptr;

	m_blendState = nullptr;
	m_depthState = nullptr;
	m_rasterizerState = nullptr;

	m_frameBuf = nullptr;
}

D3D11RenderPass::~D3D11RenderPass()
{
	SAFE_DELETE(m_shader);

	SAFE_DELETE(m_blendState);
	SAFE_DELETE(m_depthState);
	SAFE_DELETE(m_rasterizerState);

	SAFE_DELETE(m_frameBuf);
}

bool D3D11RenderPass::CreateShaderProgram(const char8* vertEnter, const char8* pixelEnter, const char8* shaderText)
{
	m_renderer = static_cast<D3D11Renderer*>(sRenderMgr->GetCurrentRenderer());

	D3D11ShaderProgram* shader = static_cast<D3D11ShaderProgram*>(m_renderer->GenerateRenderProgram());
	if (!shader->CompileVertexShader(shaderText, vertEnter) || !shader->CompilePixelShader(shaderText, pixelEnter))
	{
		SAFE_DELETE(shader);
		return false;
	}

	m_shader = shader;

	return true;
}

D3D11RenderPass* D3D11RenderPass::Clone()
{
	D3D11RenderPass* pass = new D3D11RenderPass();

	pass->m_renderer = m_renderer;

	pass->m_shader = static_cast<D3D11ShaderProgram*>(m_shader->Clone());

	if (m_blendState != nullptr) pass->m_blendState = static_cast<D3D11BlendState*>(m_blendState->Clone());
	if (m_depthState != nullptr) pass->m_depthState = static_cast<D3D11DepthStencilState*>(m_depthState->Clone());
	if (m_rasterizerState != nullptr) pass->m_rasterizerState = static_cast<D3D11RasterizerState*>(m_rasterizerState->Clone());

	if (m_frameBuf != nullptr)
	{
		int32 width = m_frameBuf->GetWidth();
		int32 height = m_frameBuf->GetHeight();
		RenderFrameBuffer* frameBuf = m_renderer->CreateFrameBuffer(width, height);
		pass->m_frameBuf = static_cast<D3D11FrameBuffer*>(frameBuf);
	}

	return pass;
}

void D3D11RenderPass::BindShaderProgram(RenderProgram* shader)
{
	m_shader = static_cast<D3D11ShaderProgram*>(shader);
}

void D3D11RenderPass::BindFrameBuffer(RenderFrameBuffer* frameBuf)
{
	m_frameBuf = static_cast<D3D11FrameBuffer*>(frameBuf);
}

void D3D11RenderPass::BindRasterizerState(RenderRasterizerState* state)
{
	m_rasterizerState = static_cast<D3D11RasterizerState*>(state);
}

void D3D11RenderPass::BindDepthStencilState(RenderDepthStencilState* state)
{
	m_depthState = static_cast<D3D11DepthStencilState*>(state);
}

void D3D11RenderPass::BindBlendState(RenderBlendState* state)
{
	m_blendState = static_cast<D3D11BlendState*>(state);
}

void D3D11RenderPass::ApplyToDevice()
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