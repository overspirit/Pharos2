#include "PreCompile.h"
#include "Pharos.h"

MetalRenderCommand::MetalRenderCommand(IMetalCommandFactory* cmdFactory, MetalRenderTarget* renderTarget)
{
	m_cmdFactory = cmdFactory;
	m_renderTarget = renderTarget;
}

MetalRenderCommand::~MetalRenderCommand()
{
}

void MetalRenderCommand::BeginCommand()
{
	if (m_renderEncoder != nil) return;
	
	MTLRenderPassDescriptor* renderPassDesc = m_renderTarget->GetMetalPassDescriptor();
	
	id<MTLCommandBuffer> commandBuffer = m_cmdFactory->MakeMetalCommandBuffer();
	
	m_renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor : renderPassDesc];
	
	m_renderEncoder.label = @"MyRenderEncoder";
	[m_renderEncoder pushDebugGroup : [NSString stringWithUTF8String:m_debugLabel.c_str()] ];
	[m_renderEncoder setFrontFacingWinding : MTLWindingCounterClockwise];
	[m_renderEncoder setCullMode : MTLCullModeFront];
}

void MetalRenderCommand::SetVertexBuffer(RenderBuffer* buffer)
{
	if (buffer->GetBufferType() != VERTEX_BUFFER) return;
	
	MetalRenderBuffer* metalBuf = static_cast<MetalRenderBuffer*>(buffer);
	if (metalBuf != nullptr)
	{
		//metalBuf->ApplyVertexBuffer(VERTEX_BUFFER_BEGIN_SLOT, m_renderEncoder);
		
		id<MTLBuffer> vertexBuffer = metalBuf->GetMetalBuffer();
		
		[m_renderEncoder setVertexBuffer:vertexBuffer offset:0 atIndex:VERTEX_BUFFER_BEGIN_SLOT];
	}
}

void MetalRenderCommand::SetIndexBuffer(RenderBuffer* indexBuffer, IndexElementType indexType)
{
	const static MTLIndexType metalIndexType[] =
	{
		MTLIndexTypeUInt32,
		MTLIndexTypeUInt16,
	};
	
	m_indexBufType = metalIndexType[indexType];
	
	MetalRenderBuffer* metalBuf = static_cast<MetalRenderBuffer*>(indexBuffer);
	if (metalBuf != nullptr)
	{
		m_indexBuffer = metalBuf->GetMetalBuffer();
	}
}

void MetalRenderCommand::SetViewport(const Rect2Di& viewRect, float32 minDepth, float32 maxDepth)
{
	
}

void MetalRenderCommand::SetScissorRect(const Rect2Di& scissorRect)
{
	
}

void MetalRenderCommand::SetPipeline(RenderResourceSet* resSet, RenderPipeline* pipeline)
{
	MetalResourceSet* metalResSet = static_cast<MetalResourceSet*>(resSet);
	if (metalResSet != nullptr)
	{
		metalResSet->ApplyResourceSet(m_renderEncoder);
	}
	
	MetalRenderPipeline* metalPipeline = static_cast<MetalRenderPipeline*>(pipeline);
	if (metalPipeline != nullptr)
	{
		EPixelFormat colorFmt[8];
		for(int i = 0; i < 8; i++)
		{
			colorFmt[i] = m_renderTarget->GetColorAttachFormat(i);
		}
		
		EPixelFormat depthFmt = m_renderTarget->GetDepthAttachFormat();
		EPixelFormat stencilFmt = m_renderTarget->GetStencilAttachFormat();
		
		metalPipeline->SetTargetFormat(colorFmt, depthFmt, stencilFmt);
		metalPipeline->ApplyToEncoder(m_renderEncoder);
		
		m_currDrawType = metalPipeline->GetDrawType();
	}
}

void MetalRenderCommand::DrawPrimitives(uint32 start, uint32 count)
{
	CHECK_ENUM(0, EDT_POINTLIST);
	CHECK_ENUM(1, EDT_LINELIST);
	CHECK_ENUM(2, EDT_LINESTRIP);
	CHECK_ENUM(3, EDT_TRIANGLELIST);
	CHECK_ENUM(4, EDT_TRIANGLESTRIP);
	
	const static MTLPrimitiveType prim[] =
	{
		MTLPrimitiveTypePoint,
		MTLPrimitiveTypeLine,
		MTLPrimitiveTypeLineStrip,
		MTLPrimitiveTypeTriangle,
		MTLPrimitiveTypeTriangleStrip,
	};
	
	[m_renderEncoder drawPrimitives : prim[m_currDrawType] vertexStart : start vertexCount : count];
}

void MetalRenderCommand::DrawIndexedPrimitives(uint32 start, uint32 count)
{
	CHECK_ENUM(0, EDT_POINTLIST);
	CHECK_ENUM(1, EDT_LINELIST);
	CHECK_ENUM(2, EDT_LINESTRIP);
	CHECK_ENUM(3, EDT_TRIANGLELIST);
	CHECK_ENUM(4, EDT_TRIANGLESTRIP);
	
	const static MTLPrimitiveType prim[] =
	{
		MTLPrimitiveTypePoint,
		MTLPrimitiveTypeLine,
		MTLPrimitiveTypeLineStrip,
		MTLPrimitiveTypeTriangle,
		MTLPrimitiveTypeTriangleStrip,
	};
	
	[m_renderEncoder drawIndexedPrimitives:prim[m_currDrawType] indexCount:count indexType:m_indexBufType indexBuffer:m_indexBuffer indexBufferOffset:start];
}
	 
void MetalRenderCommand::EndCommand()
{
	[m_renderEncoder popDebugGroup];
	[m_renderEncoder endEncoding];
	
	m_renderEncoder = nil;
}
