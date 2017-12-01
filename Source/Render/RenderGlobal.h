#pragma once

#include "Pharos.h"

// #include "Effect/RenderVariable.h"
// #include "Effect/RenderTechnique.h"
// #include "Effect/RenderEffectLoader.h"
// #include "Effect/RenderBlock.h"
// #include "Renderer/Renderer.h"

#if defined(_WINDOWS_PLATFORM_)

// #include "Renderer/D3D11/D3D11Utils.h"
// 
// #include "Renderer/D3D11/D3D11SampleState.h"
// #include "Renderer/D3D11/D3D11BlendState.h"
// #include "Renderer/D3D11/D3D11DepthStencilState.h"
// #include "Renderer/D3D11/D3D11RasterizerState.h"
// #include "Renderer/D3D11/D3D11Texture.h"
#include "Renderer/D3D11/D3D11FrameBuffer.h"
#include "Renderer/D3D11/D3D11ConstantBuffer.h"
#include "Renderer/D3D11/D3D11RenderLayout.h"
#include "Renderer/D3D11/D3D11ShaderProgram.h"
// #include "Renderer/D3D11/D3D11Renderer.h"
// 
// #include "Effect/D3D11/D3D11EffectLoader.h"
// #include "Effect/D3D11/D3D11RenderPass.h"
// #include "Effect/D3D11/D3D11RenderTechnique.h"
// #include "Effect/D3D11/D3D11RenderBlock.h"

#define MakeRenderer() MakeSharedPtr<D3D11Renderer>()
#define MakeEffectLoader() MakeSharedPtr<D3D11EffectLoader>()

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

// #include "RenderSprite.h"
// #include "RenderImage.h"
// #include "RenderFont.h"
// #include "Material.h"
// #include "Mesh.h"
// #include "Model.h"
// #include "RenderMgr.h"

#define sRenderMgr RenderMgr::Inst()

#define sRenderSpirite RenderSprite::Inst()

using namespace Render;
