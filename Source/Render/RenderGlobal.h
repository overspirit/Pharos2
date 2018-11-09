#pragma once

#include "Pharos.h"

#include "RenderDefine.h"
#include "VertexDefine.h"

#include "Renderer/RenderSamplerState.h"
#include "Renderer/RenderBlendState.h"
#include "Renderer/RenderDepthStencilState.h"
#include "Renderer/RenderRasterizerState.h"
#include "Renderer/RenderTexture.h"
#include "Renderer/RenderFrameBuffer.h"
#include "Renderer/RenderShaderData.h"
#include "Renderer/RenderProgram.h"
#include "Renderer/RenderLayout.h"
#include "Renderer/Renderer.h"

#include "Effect/RenderTechInfo.h"
#include "Effect/RenderEffectLoader.h"
#include "Effect/RenderVariable.h"
#include "Effect/RenderPass.h"
#include "Effect/RenderTechnique.h"

#if defined(_WINDOWS_PLATFORM_)

#include "Renderer/D3D11/D3D11Utils.h"
#include "Renderer/D3D11/D3D11SamplerState.h"
#include "Renderer/D3D11/D3D11BlendState.h"
#include "Renderer/D3D11/D3D11DepthStencilState.h"
#include "Renderer/D3D11/D3D11RasterizerState.h"
#include "Renderer/D3D11/D3D11Texture.h"
#include "Renderer/D3D11/D3D11FrameBuffer.h"
#include "Renderer/D3D11/D3D11ConstantBuffer.h"
#include "Renderer/D3D11/D3D11RenderLayout.h"
#include "Renderer/D3D11/D3D11ShaderProgram.h"
#include "Renderer/D3D11/D3D11Renderer.h"

#include "Effect/D3D11/D3D11RenderTechInfo.h"
#include "Effect/D3D11/D3D11EffectLoader.h"
#include "Effect/D3D11/D3D11RenderPass.h"
#include "Effect/D3D11/D3D11RenderTechnique.h"

#define MakeRenderer() new D3D11Renderer()
#define MakeEffectLoader() new D3D11EffectLoader()
#define MakeRenderTechnique() new D3D11RenderTechnique()

#endif

#if defined(_ANDROID_PLATFORM_)

#include "Renderer/OpenGL/OpenGLUtils.h"
#include "Renderer/OpenGL/OpenGLBlendState.h"
#include "Renderer/OpenGL/OpenGLDepthStencilState.h"
#include "Renderer/OpenGL/OpenGLRasterizerState.h"
#include "Renderer/OpenGL/OpenGLSampleState.h"
#include "Renderer/OpenGL/OpenGLTexture.h"
#include "Renderer/OpenGL/OpenGLFrameBuf.h"
#include "Renderer/OpenGL/OpenGLUniformBuf.h"
#include "Renderer/OpenGL/OpenGLRenderPass.h"
#include "Renderer/OpenGL/OpenGLRenderTechnique.h"
#include "Renderer/OpenGL/OpenGLRenderLayout.h"
#include "Renderer/OpenGL/OpenGLRenderBlock.h"
#include "Renderer/OpenGL/OpenGLEffectLoader.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Renderer/OpenGL/OGLAndroidRenderer.h"

#endif

#if defined(_IOS_PLATFORM_)

#include "Renderer/OpenGL/OpenGLUtils.h"
#include "Renderer/OpenGL/OpenGLEffectLoader.h"
#include "Renderer/OpenGL/OpenGLBlendState.h"
#include "Renderer/OpenGL/OpenGLDepthStencilState.h"
#include "Renderer/OpenGL/OpenGLRasterizerState.h"
#include "Renderer/OpenGL/OpenGLSampleState.h"
#include "Renderer/OpenGL/OpenGLTexture.h"
#include "Renderer/OpenGL/OpenGLFrameBuf.h"
#include "Renderer/OpenGL/OpenGLUniformBuf.h"
#include "Renderer/OpenGL/OpenGLRenderPass.h"
#include "Renderer/OpenGL/OpenGLRenderTechnique.h"
#include "Renderer/OpenGL/OpenGLRenderLayout.h"
#include "Renderer/OpenGL/OpenGLRenderBlock.h"
#include "Renderer/OpenGL/OpenGLContextIOS.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"

#define MakeRenderer() MakeSharedPtr<OpenGLRenderer>();
#define MakeEffectLoader() MakeSharedPtr<OpenGLEffectLoader>()

#endif

#include "PostProcess/PostProcess.h"
#include "PostProcess/ImageStatPostProcess.h"
#include "PostProcess/LensEffectsPostProcess.h"
#include "PostProcess/ToneMappingPostProcess.h"
#include "PostProcess/HDRPostProcess.h"

#include "IRenderCallback.h"
#include "RenderBlock.h"

#include "Material.h"
#include "Mesh.h"
#include "Model.h"

#include "RenderMgr.h"

#define sRenderMgr RenderMgr::Inst()

using namespace Render;
