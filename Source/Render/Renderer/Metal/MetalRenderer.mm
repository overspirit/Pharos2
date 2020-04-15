#include "PreCompile.h"
#include "Pharos.h"

Pharos::Render::Renderer* MakeMetalRenderer() { return new Pharos::Render::MetalRenderer(); }
Pharos::Render::RenderEffectLoader* MakeMetalEffectLoader() { return new Pharos::Render::MetalEffectLoader(); }
Pharos::Render::RenderTechnique* MakeMetalRenderTechnique() { return new Pharos::Render::MetalRenderTechnique(); }

MetalRenderer::MetalRenderer(void)
{
}

MetalRenderer::~MetalRenderer(void)
{
}

bool MetalRenderer::Init()
{
	return true;
}

void MetalRenderer::Destroy()
{
}

bool MetalRenderer::Create(const DeviceConfig& cfg)
{
    m_device = MTLCreateSystemDefaultDevice();
    
    MTKView* view = (__bridge id)sKernel->GetWindowHandle();
    view.device = m_device;
    view.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    view.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
    view.sampleCount = 1;
    view.clearColor = MTLClearColorMake(0.5, 0.5, 0.5, 0.5);
    
    m_commandQueue = [m_device newCommandQueue];
    
    MTLRenderPassDescriptor* renderPassDescriptor = view.currentRenderPassDescriptor;
    
    view.currentDrawable;
    
    m_defFrameBuf = new MetalFrameBuffer();
    m_defFrameBuf->ApplyDevice();
    
	return true;
}

void MetalRenderer::BindFrameBuffer(RenderFrameBuffer* frameBuf)
{
}

void MetalRenderer::BindLayout(RenderLayout* layout)
{
}

void MetalRenderer::BindShaderData(uint32 slot, RenderShaderData* data)
{
}

void MetalRenderer::BindTexture(uint32 slot, RenderTexture* tex)
{
}

void MetalRenderer::BindProgram(RenderProgram* program)
{
}

void MetalRenderer::BindBlendState(RenderBlendState* state)
{
}

void MetalRenderer::BindRasterizerState(RenderRasterizerState* state)
{
}

void MetalRenderer::BindDepthStencilState(RenderDepthStencilState* state)
{
}

void MetalRenderer::DrawImmediate(DrawType type, uint32 start, uint32 count)
{
}

bool MetalRenderer::Present()
{
    
    /// Per frame updates here
    
    
    //dispatch_semaphore_wait(_inFlightSemaphore, DISPATCH_TIME_FOREVER);
    
    id <MTLCommandBuffer> commandBuffer = [m_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";
    
    //__block dispatch_semaphore_t block_sema = _inFlightSemaphore;
    //[commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer)
    // {
    //     dispatch_semaphore_signal(block_sema);
    // }];
    
    //updateDynamicBufferState();
    
    //updateGameState();
    
    /// Delay getting the currentRenderPassDescriptor until we absolutely need it to avoid
    ///   holding onto the drawable and blocking the display pipeline any longer than necessary
    
    
    //if(renderPassDescriptor != nil) {
        
        /// Final pass rendering code here
        
        id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor: renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";
        
        [renderEncoder pushDebugGroup:@"DrawBox"];
        
        [renderEncoder setFrontFacingWinding:MTLWindingCounterClockwise];
        [renderEncoder setCullMode:MTLCullModeBack];
        [renderEncoder setRenderPipelineState:_pipelineState];
        [renderEncoder setDepthStencilState:_depthState];
    
    
    
    
//        [renderEncoder setVertexBuffer:_dynamicUniformBuffer offset:_uniformBufferOffset atIndex:BufferIndexUniforms];
//        [renderEncoder setFragmentBuffer:_dynamicUniformBuffer offset:_uniformBufferOffset atIndex:BufferIndexUniforms];
//
//
//        for (NSUInteger bufferIndex = 0; bufferIndex < _mesh.vertexBuffers.count; bufferIndex++)
//        {
//            MTKMeshBuffer *vertexBuffer = _mesh.vertexBuffers[bufferIndex];
//            if((NSNull*)vertexBuffer != [NSNull null])
//            {
//                [renderEncoder setVertexBuffer:vertexBuffer.buffer
//                                        offset:vertexBuffer.offset
//                                       atIndex:bufferIndex];
//            }
//        }
//
//        [renderEncoder setFragmentTexture:_colorMap
//                                  atIndex:TextureIndexColor];
//
//        for(MTKSubmesh *submesh in _mesh.submeshes)
//        {
//            [renderEncoder drawIndexedPrimitives:submesh.primitiveType
//                                      indexCount:submesh.indexCount
//                                       indexType:submesh.indexType
//                                     indexBuffer:submesh.indexBuffer.buffer
//                               indexBufferOffset:submesh.indexBuffer.offset];
//        }
    
        [renderEncoder popDebugGroup];
        
        [renderEncoder endEncoding];
        
        [commandBuffer presentDrawable: view.currentDrawable];
    //}
    
    [commandBuffer commit];

    return true;
}

RenderProgram* MetalRenderer::GenerateRenderProgram()
{
    return new MetalShaderProgram();
}

RenderLayout* MetalRenderer::GenerateRenderLayout(uint32 vertSize, MemoryBuffer* buf)
{
    MetalRenderLayout* layout = new MetalRenderLayout();
    if (!layout->CreateVertexBuffer(vertSize, buf))
    {
        SAFE_DELETE(layout);
        return nullptr;
    }
    return layout;
}

RenderTexture* MetalRenderer::CreateTexture(int32 width, int32 height, EPixelFormat fmt)
{
    MetalTexture* texture = new MetalTexture();
    if (!texture->Create(width, height, fmt))
    {
        SAFE_DELETE(texture);
        return nullptr;
    }
    
    return texture;
}

RenderTexture* MetalRenderer::CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt)
{
    MetalTexture* texture = new MetalTexture();
    if (!texture->CreateTargetTexture(width, height, fmt))
    {
        SAFE_DELETE(texture);
        return nullptr;
    }
    
    return texture;
}

RenderTexture* MetalRenderer::CreateDepthTexture(int32 width, int32 height)
{
    MetalTexture* texture = new MetalTexture();
    if (!texture->CreateDepthTexture(width, height))
    {
        SAFE_DELETE(texture);
        return nullptr;
    }
    
    return texture;
}

RenderTexture* MetalRenderer::LoadTexture(const char8* szPath)
{
    MetalTexture* texture = new MetalTexture();
    if (!texture->LoadFromFile(szPath))
    {
        SAFE_DELETE(texture);
        return nullptr;
    }
    
    return texture;
}

RenderTexture* MetalRenderer::LoadTexture(const Image* image)
{
    MetalTexture* texture = new MetalTexture();
    if (!texture->LoadFromImage(image))
    {
        SAFE_DELETE(texture);
        return nullptr;
    }
    
    return texture;
}

RenderShaderData* MetalRenderer::CreateShaderData()
{
    return new MetalConstantBuffer();
}

RenderFrameBuffer* MetalRenderer::CreateFrameBuffer(int32 width, int32 height)
{
    MetalFrameBuffer* frameBuf = new MetalFrameBuffer();
    if (!frameBuf->InitFrameBuffer(width, height))
    {
        SAFE_DELETE(frameBuf);
        return nullptr;
    }
    
    return frameBuf;
}

RenderSamplerState* MetalRenderer::CreateSampleState(const SamplerStateDesc& desc)
{
    MetalSamplerState* state = new MetalSamplerState();
    if (!state->CreateState(desc))
    {
        SAFE_DELETE(state);
        return nullptr;
    }
    return state;
}

RenderBlendState* MetalRenderer::CreateBlendState(const BlendStateDesc& desc)
{
    MetalBlendState* state = new MetalBlendState();
    if (!state->CreateState(desc))
    {
        SAFE_DELETE(state);
        return nullptr;
    }
    return state;
}

RenderRasterizerState* MetalRenderer::CreateRasterizerState(const RasterizerStateDesc& desc)
{
    MetalRasterizerState* state = new MetalRasterizerState();
    if (!state->CreateState(desc))
    {
        SAFE_DELETE(state);
        return nullptr;
    }
    return state;
}

RenderDepthStencilState* MetalRenderer::CreateDepthStencilState(const DepthStencilStateDesc& desc)
{
    MetalDepthStencilState* state = new MetalDepthStencilState();
    if (!state->CreateState(desc))
    {
        SAFE_DELETE(state);
        return nullptr;
    }
    return state;
}
