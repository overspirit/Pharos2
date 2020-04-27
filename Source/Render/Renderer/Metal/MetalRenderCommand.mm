#include "PreCompile.h"
#include "Pharos.h"

MetalRenderCommand::MetalRenderCommand(id<MTLCommandQueue> commandQueue, MTKView* view)
{
	m_commandQueue = commandQueue;
	m_view = view;
}

MetalRenderCommand::~MetalRenderCommand()
{
}

bool MetalRenderCommand::Init()
{
	//m_renderPass = [[MTLRenderPassDescriptor alloc] init];
	//m_renderPass.colorAttachments[0].clearColor = MTLClearColorMake(0.5, 0.5, 0.5, 0.5);

	m_renderPassDescriptor = m_view.currentRenderPassDescriptor;
	MTLRenderPassColorAttachmentDescriptorArray* colorAttachments = m_renderPassDescriptor.colorAttachments;
	MTLRenderPassDepthAttachmentDescriptor* depthAttachment = m_renderPassDescriptor.depthAttachment;
	MTLRenderPassStencilAttachmentDescriptor* stencilAttachment = m_renderPassDescriptor.stencilAttachment;

	return true;
}

void MetalRenderCommand::BuildEncoder()
{
	if (m_commandBuffer != nullptr && m_renderEncoder != nullptr) return;

	m_commandBuffer = [m_commandQueue commandBuffer];
	m_commandBuffer.label = @"MyCommand";

		m_renderPassDescriptor = m_view.currentRenderPassDescriptor;    //pass 每帧都在变化，必须重新获取...
	m_renderEncoder = [m_commandBuffer renderCommandEncoderWithDescriptor : m_renderPassDescriptor];

	m_renderEncoder.label = @"MyRenderEncoder";
		[m_renderEncoder pushDebugGroup : @"DrawBox"];
	[m_renderEncoder setFrontFacingWinding : MTLWindingClockwise];
	[m_renderEncoder setCullMode : MTLCullModeBack];
}

void MetalRenderCommand::SetVertexBuffer(uint32 slot, RenderBuffer* buffer)
{
	BuildEncoder();

	MetalRenderBuffer* metalBuf = static_cast<MetalRenderBuffer*>(buffer);
	if (metalBuf != nullptr)
	{
		id<MTLBuffer> vertBuf = metalBuf->GetMetalBuffer();

		[m_renderEncoder setVertexBuffer : vertBuf offset : 0 atIndex : slot];
	}
}

void MetalRenderCommand::SetFragmentBuffer(uint32 slot, RenderBuffer* buffer)
{
	BuildEncoder();
}

void MetalRenderCommand::SetFragmentTexture(uint32 slot, RenderTexture* tex)
{
	BuildEncoder();

	MetalTexture* metalTex = static_cast<MetalTexture*>(tex);
	if (metalTex != nullptr)
	{
		id<MTLTexture> texture = metalTex->GetMetalTexture();

		[m_renderEncoder setFragmentTexture : texture atIndex : 0];
	}
}

void MetalRenderCommand::SetPipeline(RenderPipeline* pipeline)
{
	BuildEncoder();

	MetalRenderPipeline* metalPipeline = static_cast<MetalRenderPipeline*>(pipeline);
	if (metalPipeline != nullptr)
	{
		id<MTLRenderPipelineState> pipeline = metalPipeline->GetMetalPipeline();

		[m_renderEncoder setRenderPipelineState : pipeline];
	}
}

//void MetalRenderer::BindPass(RenderPass* pass)
//{
//    MetalRenderPass* metalPass = static_cast<MetalRenderPass*>(pass);
//    if (metalPass != nullptr)
//    {
//        m_pass = metalPass;
//    }
//}

void MetalRenderCommand::DrawImmediate(DrawType type, uint32 start, uint32 count)
{
	BuildEncoder();

	[m_renderEncoder drawPrimitives : MTLPrimitiveTypeTriangle vertexStart : 0 vertexCount : 6];
}

bool MetalRenderCommand::Present()
{

	/// Per frame updates here


	//dispatch_semaphore_wait(_inFlightSemaphore, DISPATCH_TIME_FOREVER);

	;

	//__block dispatch_semaphore_t block_sema = _inFlightSemaphore;
	//[commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer)
	// {
	//     dispatch_semaphore_signal(block_sema);
	// }];


	[m_renderEncoder popDebugGroup];
	[m_renderEncoder endEncoding];

	[m_commandBuffer presentDrawable : m_view.currentDrawable];

	[m_commandBuffer commit];

	m_renderEncoder = nullptr;
	m_commandBuffer = nullptr;

	return true;
}
