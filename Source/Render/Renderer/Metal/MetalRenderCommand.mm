#include "PreCompile.h"
#include "Pharos.h"

MetalRenderCommand::MetalRenderCommand(IMetalCommandFactory* cmdFactory)
{
	m_cmdFactory = cmdFactory;
    
    m_metalTarget = NULL;
    
    m_indexBufType = MTLIndexTypeUInt32;
    
    m_currDrawType = EDT_TRIANGLELIST;
}

MetalRenderCommand::~MetalRenderCommand()
{
}

void MetalRenderCommand::BeginCommand()
{
	if (m_renderEncoder != nil) return;
	
    m_commandBuffer = m_cmdFactory->MakeMetalCommandBuffer();
}

void MetalRenderCommand::EndCommand()
{
    m_commandBuffer = nil;
    m_renderEncoder = nil;
    
    m_indexBuffer = nil;
}

bool MetalRenderCommand::BeginRenderTarget(RenderTarget* target)
{
    if (m_commandBuffer == nil) return false;
    
    m_metalTarget = (MetalRenderTarget*)target;
    
    MTLRenderPassDescriptor* renderPassDesc = m_metalTarget->GetMetalPassDescriptor();
    
    m_renderEncoder = [m_commandBuffer renderCommandEncoderWithDescriptor : renderPassDesc];
    
    m_renderEncoder.label = [NSString stringWithUTF8String:m_debugLabel.c_str()];
    [m_renderEncoder pushDebugGroup : [NSString stringWithUTF8String:m_debugLabel.c_str()] ];
    [m_renderEncoder setFrontFacingWinding : MTLWindingCounterClockwise];
    [m_renderEncoder setCullMode : MTLCullModeNone];
    
    return true;
}

void MetalRenderCommand::EndRenderTarget()
{
    [m_renderEncoder popDebugGroup];
    [m_renderEncoder endEncoding];
}

void MetalRenderCommand::SetVertexBuffer(RenderBuffer* buffer)
{
	if (buffer->GetBufferType() != VERTEX_BUFFER) return;
	
	MetalRenderBuffer* metalBuf = static_cast<MetalRenderBuffer*>(buffer);
	if (metalBuf != nullptr)
	{		
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
    MTLViewport viewport;
    viewport.originX = viewRect.left;
    viewport.originY = viewRect.top;
    viewport.width = viewRect.GetWidth();
    viewport.height = viewRect.GetHeight();
    viewport.znear = minDepth;
    viewport.zfar = maxDepth;
    
    [m_renderEncoder setViewport: viewport];
}

void MetalRenderCommand::SetScissorRect(const Rect2Di& scissorRect)
{
    MTLScissorRect rect;
    rect.x = scissorRect.left;
    rect.y = scissorRect.top;
    rect.width = scissorRect.GetWidth();
    rect.height = scissorRect.GetHeight();
    
    [m_renderEncoder setScissorRect: rect];
}

void MetalRenderCommand::SetRenderStaging(RenderResourceSet* resSet, RenderPipeline* pipeline)
{
	MetalResourceSet* metalResSet = static_cast<MetalResourceSet*>(resSet);
	if (metalResSet != nullptr)
	{
		metalResSet->ApplyResourceSet(m_renderEncoder);
	}
	
	MetalRenderPipeline* metalPipeline = static_cast<MetalRenderPipeline*>(pipeline);
	if (metalPipeline != nullptr)
	{
        m_currDrawType = metalPipeline->GetDrawType();
        
        EPixelFormat colorFmt[8];
        for (int i = 0; i < 8; i++)
        {
            colorFmt[i] = m_metalTarget->GetColorAttachFormat(i);
        }
        
        EPixelFormat depthStencilFmt = m_metalTarget->GetDepthAttachFormat();
        
        metalPipeline->SetTargetFormat(colorFmt, depthStencilFmt, depthStencilFmt);
        
		metalPipeline->ApplyToEncoder(m_renderEncoder);
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

