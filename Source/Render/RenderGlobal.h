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
#include "Renderer/RenderResourceSet.h"
#include "Renderer/RenderProgram.h"
#include "Renderer/RenderPipeline.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"


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

#endif

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

#endif
//////////////////////////////////////////////////////////////////////////


//Linux or Android
//////////////////////////////////////////////////////////////////////////
#if defined(_LINUX_PLATFORM_) || defined(_ANDROID_PLATFORM_)

#include "Renderer/Vulkan/VulkanUtils.h"
#include "Renderer/Vulkan/VulkanInitializeHelper.h"
#include "Renderer/Vulkan/VulkanBlendState.h"
#include "Renderer/Vulkan/VulkanDepthStencilState.h"
#include "Renderer/Vulkan/VulkanRasterizerState.h"
#include "Renderer/Vulkan/VulkanSamplerState.h"
#include "Renderer/Vulkan/VulkanRenderTexture.h"
#include "Renderer/Vulkan/VulkanRenderTarget.h"
#include "Renderer/Vulkan/VulkanDefaultTarget.h"
#include "Renderer/Vulkan/VulkanRenderBuffer.h"
#include "Renderer/Vulkan/VulkanDescriptorSet.h"
#include "Renderer/Vulkan/VulkanShaderProgram.h"
#include "Renderer/Vulkan/VulkanRenderPipeline.h"
#include "Renderer/Vulkan/VulkanRenderCommand.h"
#include "Renderer/Vulkan/VulkanRenderer.h"

#define sInitHelper VulkanInitializeHelper::Inst()

#endif
//////////////////////////////////////////////////////////////////////////

#include "RenderObject.h"
#include "RenderVariable.h"
#include "Material.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Animation.h"
#include "Model.h"
#include "IRenderCallback.h"
#include "MaterialMgr.h"
#include "RenderMgr.h"

#define sRenderMgr RenderMgr::Inst()
#define sMaterialMgr MaterialMgr::Inst()
#define sRenderer (sRenderMgr->GetCurrentRenderer())

Pharos::Render::Renderer* MakeRenderer();
#define IMPL_MAKE_RENDERER(renderer) Pharos::Render::Renderer* MakeRenderer(){return new renderer();}

using namespace Render;
