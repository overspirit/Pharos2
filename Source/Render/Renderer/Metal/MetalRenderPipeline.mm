#include "PreCompile.h"
#include "Pharos.h"

MetalRenderPipeline::MetalRenderPipeline(id<MTLDevice> device)
{
	m_device = device;
	m_stateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
	
	m_depthState = nullptr;
}

MetalRenderPipeline::~MetalRenderPipeline()
{
}

bool MetalRenderPipeline::SetInputLayoutDesc(const VertLayoutDesc* desc, uint32 descNum)
{
	if(m_pipelineState != nullptr)
	{
		m_pipelineState = nullptr;
	}
	
	MTLVertexDescriptor* mtlVertexDescriptor = [[MTLVertexDescriptor alloc] init];

	for (uint32 i = 0; i < descNum; i++)
	{
		MTLVertexFormat vertFormat = GetMetalFormat(desc[i].elementType, desc[i].elementNum);
		uint32 elementSize = GetVertElementSize(desc[i].elementType, desc[i].elementNum);
		uint32 elementLocation = desc[i].elementLocation;
		uint32 elementOffset = desc[i].elementOffset;
		uint32 layoutIndex = desc[i].layoutIndex + VERTEX_BUFFER_BEGIN_SLOT;

		mtlVertexDescriptor.attributes[elementLocation].format = vertFormat;
		mtlVertexDescriptor.attributes[elementLocation].offset = elementOffset;
		mtlVertexDescriptor.attributes[elementLocation].bufferIndex = layoutIndex;

		mtlVertexDescriptor.layouts[layoutIndex].stride += elementSize;
		mtlVertexDescriptor.layouts[layoutIndex].stepRate = 1;
		mtlVertexDescriptor.layouts[layoutIndex].stepFunction = MTLVertexStepFunctionPerVertex;
	}
	
	m_stateDescriptor.vertexDescriptor = mtlVertexDescriptor;
	
	return true;
}

void MetalRenderPipeline::SetTargetFormat(EPixelFormat colorFmt[], EPixelFormat depthFmt, EPixelFormat stencilFmt)
{	
	if(m_pipelineState != nullptr)
	{
		m_pipelineState = nullptr;
	}
	
	//m_stateDescriptor.label = @"MyPipeline";
	//m_stateDescriptor.sampleCount = 1;
	
	for(int i = 0; i < 8; i++)
	{
		if(colorFmt[i] != EPF_END)
		{
			m_stateDescriptor.colorAttachments[i].pixelFormat = PixelFormat2MetalFormat(colorFmt[i]);
		}
	}
	
	if(depthFmt != EPF_END)
	{
		m_stateDescriptor.depthAttachmentPixelFormat = PixelFormat2MetalFormat(depthFmt);
	}
	
	if(stencilFmt != EPF_END)
	{
		m_stateDescriptor.stencilAttachmentPixelFormat = PixelFormat2MetalFormat(stencilFmt);
	}
}

bool MetalRenderPipeline::SetProgramShader(RenderProgram* program)
{
	if(m_pipelineState != nullptr)
	{
		m_pipelineState = nullptr;
	}
	
	MetalShaderProgram* shaderProgram = static_cast<MetalShaderProgram*>(program);
	if(shaderProgram != nullptr)
	{
		m_stateDescriptor.vertexFunction = shaderProgram->GetVertexFunction();
		m_stateDescriptor.fragmentFunction = shaderProgram->GetFragmentFunction();
	}
	
	return true;
}

void MetalRenderPipeline::SetBlendState(RenderBlendState* state)
{

}

void MetalRenderPipeline::SetRasterizerState(RenderRasterizerState* state)
{

}

void MetalRenderPipeline::SetDepthStencilState(RenderDepthStencilState* state)
{
	m_depthState = static_cast<MetalDepthStencilState*>(state);
}

void MetalRenderPipeline::ApplyToEncoder(id<MTLRenderCommandEncoder> encoder)
{
	if(m_pipelineState == nullptr)
	{
		if (m_depthState != nullptr)
		{
			m_depthState->ApplyDevice(encoder);
		}
		
		NSError* error = nil;
		m_pipelineState = [m_device newRenderPipelineStateWithDescriptor: m_stateDescriptor error: &error];
		if (!m_pipelineState)
		{
			NSLog(@"Failed to created pipeline state, error %@", error);
		}
	}
	
	[encoder setRenderPipelineState : m_pipelineState];
}

MTLVertexFormat MetalRenderPipeline::GetMetalFormat(VertElementType elementType, uint32 elementNum)
{
	switch (elementType)
	{
			case Pharos::Render::VET_FLOAT32:
		{
			switch (elementNum)
			{
					case 1: return MTLVertexFormatFloat;
					case 2: return MTLVertexFormatFloat2;
					case 3: return MTLVertexFormatFloat3;
					case 4: return MTLVertexFormatFloat4;
			}
		}
			break;
			case Pharos::Render::VET_FLOAT16:
		{
			switch (elementNum)
			{
					case 1: return MTLVertexFormatHalf;
					case 2: return MTLVertexFormatHalf2;
					case 3: return MTLVertexFormatHalf3;
					case 4: return MTLVertexFormatHalf4;
			}
		}
			break;
			case Pharos::Render::VET_SINT32:
		{
			switch (elementNum)
			{
					case 1: return MTLVertexFormatInt;
					case 2: return MTLVertexFormatInt2;
					case 3: return MTLVertexFormatInt3;
					case 4: return MTLVertexFormatInt4;
			}
		}
			break;
			case Pharos::Render::VET_SINT16:
		{
			switch (elementNum)
			{
					case 1: return MTLVertexFormatShort;
					case 2: return MTLVertexFormatShort2;
					case 3: return MTLVertexFormatShort3;
					case 4: return MTLVertexFormatShort4;
			}
		}
			break;
			case Pharos::Render::VET_SINT8:
		{
			switch (elementNum)
			{
					case 1: return MTLVertexFormatChar;
					case 2: return MTLVertexFormatChar2;
					case 3: return MTLVertexFormatChar3;
					case 4: return MTLVertexFormatChar4;
			}
		}
			break;
			case Pharos::Render::VET_UINT32:
		{
			switch (elementNum)
			{
					case 1: return MTLVertexFormatUInt;
					case 2: return MTLVertexFormatUInt2;
					case 3: return MTLVertexFormatUInt3;
					case 4: return MTLVertexFormatUInt4;
			}
		}
			break;
			case Pharos::Render::VET_UINT16:
		{
			switch (elementNum)
			{
					case 1: return MTLVertexFormatUShort;
					case 2: return MTLVertexFormatUShort2;
					case 3: return MTLVertexFormatUShort3;
					case 4: return MTLVertexFormatUShort4;
			}
		}
			break;
			case Pharos::Render::VET_UINT8:
		{
			switch (elementNum)
			{
					case 1: return MTLVertexFormatUChar;
					case 2: return MTLVertexFormatUChar2;
					case 3: return MTLVertexFormatUChar3;
					case 4: return MTLVertexFormatUChar4;
			}
		}
			break;
			case Pharos::Render::VET_SNORM16:
		{
			switch (elementNum)
			{
					case 1: return MTLVertexFormatShortNormalized;
					case 2: return MTLVertexFormatShort2Normalized;
					case 3: return MTLVertexFormatShort3Normalized;
					case 4: return MTLVertexFormatShort4Normalized;
			}
		}
			break;
			case Pharos::Render::VET_SNORM8:
		{
			switch (elementNum)
			{
					case 1: return MTLVertexFormatCharNormalized;
					case 2: return MTLVertexFormatChar2Normalized;
					case 3: return MTLVertexFormatChar3Normalized;
					case 4: return MTLVertexFormatChar4Normalized;
			}
		}
			break;
			case Pharos::Render::VET_UNORM16:
		{
			switch (elementNum)
			{
					case 1: return MTLVertexFormatUShortNormalized;
					case 2: return MTLVertexFormatUShort2Normalized;
					case 3: return MTLVertexFormatUShort3Normalized;
					case 4: return MTLVertexFormatUShort4Normalized;
			}
		}
			break;
			case Pharos::Render::VET_UNORM8:
		{
			switch (elementNum)
			{
					case 1: return MTLVertexFormatUCharNormalized;
					case 2: return MTLVertexFormatUChar2Normalized;
					case 3: return MTLVertexFormatUChar3Normalized;
					case 4: return MTLVertexFormatUChar4Normalized;
			}
		}
			break;
	}
	
	return MTLVertexFormatInvalid;
}

uint32 MetalRenderPipeline::GetVertElementSize(VertElementType elementType, uint32 elementNum)
{
	uint32 elementBitNum = 0;
	
	switch (elementType)
	{
			case Pharos::Render::VET_FLOAT32:
			case Pharos::Render::VET_UINT32:
			case Pharos::Render::VET_SINT32:
			elementBitNum = 4;
			break;
			case Pharos::Render::VET_FLOAT16:
			case Pharos::Render::VET_SINT16:
			case Pharos::Render::VET_UINT16:
			case Pharos::Render::VET_SNORM16:
			case Pharos::Render::VET_UNORM16:
			elementBitNum = 2;
			break;
			case Pharos::Render::VET_UINT8:
			case Pharos::Render::VET_SINT8:
			case Pharos::Render::VET_SNORM8:
			case Pharos::Render::VET_UNORM8:
			elementBitNum = 1;
			break;
	}
	
	return elementBitNum * elementNum;
}
