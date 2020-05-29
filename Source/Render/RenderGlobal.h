#pragma once

#include "RenderDefine.h"
#include "VertexDefine.h"

#include "Renderer/RenderSamplerState.h"
#include "Renderer/RenderBlendState.h"
#include "Renderer/RenderDepthStencilState.h"
#include "Renderer/RenderRasterizerState.h"
#include "Renderer/RenderTexture.h"
#include "Renderer/RenderTarget.h"
#include "Renderer/RenderBuffer.h"
#include "Renderer/RenderProgram.h"
#include "Renderer/RenderPipeline.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

#include "Effect/RenderTechInfo.h"
#include "Effect/RenderEffectLoader.h"
#include "Effect/RenderVariable.h"
#include "Effect/RenderTechnique.h"


//Windows
//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////

//macOS
//////////////////////////////////////////////////////////////////////////
#if defined(_MACOS_PLATFORM_)

#ifdef __OBJC__

#include "Renderer/Metal/MetalUtils.h"
#include "Renderer/Metal/IMetalCommandFactory.h"
#include "Renderer/Metal/MetalSamplerState.h"
#include "Renderer/Metal/MetalBlendState.h"
#include "Renderer/Metal/MetalDepthStencilState.h"
#include "Renderer/Metal/MetalRasterizerState.h"
#include "Renderer/Metal/MetalRenderTexture.h"
#include "Renderer/Metal/MetalRenderBuffer.h"
#include "Renderer/Metal/MetalRenderTarget.h"
#include "Renderer/Metal/MetalViewRenderTarget.h"
#include "Renderer/Metal/MetalShaderProgram.h"
#include "Renderer/Metal/MetalRenderPipeline.h"
#include "Renderer/Metal/MetalRenderCommand.h"
#include "Renderer/Metal/MetalRenderer.h"

#include "Effect/Metal/MetalRenderTechInfo.h"
#include "Effect/Metal/MetalEffectLoader.h"
#include "Effect/Metal/MetalRenderTechnique.h"

#endif

Pharos::Render::Renderer* MakeMetalRenderer();
Pharos::Render::RenderEffectLoader* MakeMetalEffectLoader();
Pharos::Render::RenderTechnique* MakeMetalRenderTechnique();

#define MakeRenderer() MakeMetalRenderer()
#define MakeEffectLoader() MakeMetalEffectLoader()
#define MakeRenderTechnique() MakeMetalRenderTechnique()

#endif
//////////////////////////////////////////////////////////////////////////

//iOS
//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////


//Android
//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////

//Linux
//////////////////////////////////////////////////////////////////////////
#if defined(_LINUX_PLATFORM_)

#include "Renderer/Vulkan/VulkanInitializeHelper.h"
#include "Renderer/Vulkan/VulkanBlendState.h"
#include "Renderer/Vulkan/VulkanDepthStencilState.h"
#include "Renderer/Vulkan/VulkanRasterizerState.h"
#include "Renderer/Vulkan/VulkanSamplerState.h"
#include "Renderer/Vulkan/VulkanRenderTexture.h"
#include "Renderer/Vulkan/VulkanRenderTarget.h"
#include "Renderer/Vulkan/VulkanRenderBuffer.h"
#include "Renderer/Vulkan/VulkanUniformBuffer.h"
#include "Renderer/Vulkan/VulkanRenderCommand.h"
#include "Renderer/Vulkan/VulkanRenderPipeline.h"
#include "Renderer/Vulkan/VulkanShaderProgram.h"
#include "Renderer/Vulkan/VulkanRenderer.h"

#define MakeRenderer() (new VulkanRenderer())
#define sInitHelper (VulkanInitializeHelper::Inst())

#endif
//////////////////////////////////////////////////////////////////////////

#include "RenderBlock.h"
#include "RenderObject.h"

#include "PostProcess/PostProcess.h"
#include "PostProcess/ImageStatPostProcess.h"
#include "PostProcess/LensEffectsPostProcess.h"
#include "PostProcess/ToneMappingPostProcess.h"
#include "PostProcess/HDRPostProcess.h"

#include "Material.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Animation.h"
#include "Model.h"

#include "IRenderCallback.h"
#include "RenderMgr.h"

#define sRenderMgr RenderMgr::Inst()

using namespace Render;
