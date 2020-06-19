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
	[m_renderEncoder setFrontFacingWinding : MTLWindingClockwise];
	[m_renderEncoder setCullMode : MTLCullModeBack];
}

void MetalRenderCommand::SetVertexBuffer(uint32 slot, RenderBuffer* buffer)
{
	MetalRenderBuffer* metalBuf = static_cast<MetalRenderBuffer*>(buffer);
	if (metalBuf != nullptr)
	{
		metalBuf->ApplyVertexBuffer(slot, m_renderEncoder);
	}
}

void MetalRenderCommand::SetFragmentBuffer(uint32 slot, RenderBuffer* buffer)
{
	MetalRenderBuffer* metalBuf = static_cast<MetalRenderBuffer*>(buffer);
	if (metalBuf != nullptr)
	{
		metalBuf->ApplyFragmentBuffer(slot, m_renderEncoder);
	}
}

void MetalRenderCommand::SetVertexTexture(uint32 slot, RenderTexture* tex)
{
	MetalRenderTexture* metalTex = static_cast<MetalRenderTexture*>(tex);
	if (metalTex != nullptr)
	{
		metalTex->ApplyVertexTexture(slot, m_renderEncoder);
	}
}

void MetalRenderCommand::SetFragmentTexture(uint32 slot, RenderTexture* tex)
{
	MetalRenderTexture* metalTex = static_cast<MetalRenderTexture*>(tex);
	if (metalTex != nullptr)
	{
		metalTex->ApplyFragmentTexture(slot, m_renderEncoder);
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

void MetalRenderCommand::SetPipeline(RenderPipeline* pipeline)
{
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
	}
}

void MetalRenderCommand::DrawPrimitives(DrawType type, uint32 start, uint32 count)
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
	
	[m_renderEncoder drawPrimitives : prim[type] vertexStart : start vertexCount : count];
}

void MetalRenderCommand::DrawIndexedPrimitives(DrawType type, uint32 indexCount, uint32 indexOffset)
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
	
	[m_renderEncoder drawIndexedPrimitives:prim[type] indexCount:indexCount indexType:m_indexBufType indexBuffer:m_indexBuffer indexBufferOffset:indexOffset];
}
	 
void MetalRenderCommand::EndCommand()
{
	[m_renderEncoder popDebugGroup];
	[m_renderEncoder endEncoding];
	
	m_renderEncoder = nil;
}
