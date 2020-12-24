#include "PreCompile.h"
#include "Pharos.h"

MetalRenderPipeline::MetalRenderPipeline(id<MTLDevice> device)
{
	m_device = device;
    m_pipelineStateDesc = [[MTLRenderPipelineDescriptor alloc] init];
	
	//m_depthState = nullptr;
    
    m_drawType = EDT_TRIANGLELIST;
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
	
    m_pipelineStateDesc.vertexDescriptor = mtlVertexDescriptor;
	
	return true;
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
        m_pipelineStateDesc.vertexFunction = shaderProgram->GetVertexFunction();
        m_pipelineStateDesc.fragmentFunction = shaderProgram->GetFragmentFunction();
	}
    
    MTLDepthStencilDescriptor *depthStateDesc = [[MTLDepthStencilDescriptor alloc] init];
    depthStateDesc.depthCompareFunction = MTLCompareFunctionLessEqual;
    depthStateDesc.depthWriteEnabled = YES;
	
    m_depthStencilState = [m_device newDepthStencilStateWithDescriptor:depthStateDesc];
    
	return true;
}

void MetalRenderPipeline::SetBlendState(const BlendStateDesc& state)
{
//    MTLBlendFactorZero = 0,
//    MTLBlendFactorOne = 1,
//    MTLBlendFactorSourceColor = 2,
//    MTLBlendFactorOneMinusSourceColor = 3,
//    MTLBlendFactorSourceAlpha = 4,
//    MTLBlendFactorOneMinusSourceAlpha = 5,
//    MTLBlendFactorDestinationColor = 6,
//    MTLBlendFactorOneMinusDestinationColor = 7,
//    MTLBlendFactorDestinationAlpha = 8,
//    MTLBlendFactorOneMinusDestinationAlpha = 9,
//    MTLBlendFactorSourceAlphaSaturated = 10,
//    MTLBlendFactorBlendColor = 11,
//    MTLBlendFactorOneMinusBlendColor = 12,
//    MTLBlendFactorBlendAlpha = 13,
//    MTLBlendFactorOneMinusBlendAlpha = 14,
//
//    MTLBlendOperationAdd = 0,
//    MTLBlendOperationSubtract = 1,
//    MTLBlendOperationReverseSubtract = 2,
//    MTLBlendOperationMin = 3,
//    MTLBlendOperationMax = 4,
    
    for(int i = 0; i < 8; i++)
    {
        if(m_pipelineStateDesc.colorAttachments[i].pixelFormat != MTLPixelFormatInvalid)
        {
            m_pipelineStateDesc.colorAttachments[i].blendingEnabled = true;
            m_pipelineStateDesc.colorAttachments[i].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
            m_pipelineStateDesc.colorAttachments[i].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
            m_pipelineStateDesc.colorAttachments[i].rgbBlendOperation = MTLBlendOperationAdd;
            m_pipelineStateDesc.colorAttachments[i].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
            m_pipelineStateDesc.colorAttachments[i].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
            m_pipelineStateDesc.colorAttachments[i].alphaBlendOperation = MTLBlendOperationAdd;
        }
        
    }
    
//    bool blendEnable;
//    bool alphaToCoverageEnable;
//    EBlendFunc srcBlend;
//    EBlendFunc destBlend;
//    EBlendOP blendOp;
//    EBlendFunc srcBlendAlpha;
//    EBlendFunc destBlendAlpha;
//    EBlendOP blendOpAlpha;
}

void MetalRenderPipeline::SetRasterizerState(const RasterizerStateDesc& state)
{
    
}

void MetalRenderPipeline::SetDepthStencilState(const DepthStencilStateDesc& state)
{
    MTLDepthStencilDescriptor *depthStateDesc = [[MTLDepthStencilDescriptor alloc] init];
    depthStateDesc.depthCompareFunction = MTLCompareFunctionAlways;
    depthStateDesc.depthWriteEnabled = YES;
    
    m_depthStencilState = [m_device newDepthStencilStateWithDescriptor:depthStateDesc];
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
            m_pipelineStateDesc.colorAttachments[i].pixelFormat = PixelFormat2MetalFormat(colorFmt[i]);
            
            m_pipelineStateDesc.colorAttachments[i].blendingEnabled = true;
            m_pipelineStateDesc.colorAttachments[i].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
            m_pipelineStateDesc.colorAttachments[i].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
            m_pipelineStateDesc.colorAttachments[i].rgbBlendOperation = MTLBlendOperationAdd;
            m_pipelineStateDesc.colorAttachments[i].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
            m_pipelineStateDesc.colorAttachments[i].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
            m_pipelineStateDesc.colorAttachments[i].alphaBlendOperation = MTLBlendOperationAdd;
        }
    }
    
    if(depthFmt != EPF_END)
    {
        m_pipelineStateDesc.depthAttachmentPixelFormat = PixelFormat2MetalFormat(depthFmt);
    }
    
    if(stencilFmt != EPF_END)
    {
        m_pipelineStateDesc.stencilAttachmentPixelFormat = PixelFormat2MetalFormat(stencilFmt);
    }
}

void MetalRenderPipeline::ApplyToEncoder(id<MTLRenderCommandEncoder> encoder)
{
    [encoder setDepthStencilState:m_depthStencilState];
    
	if(m_pipelineState == nullptr)
	{
        NSError* error = nil;
		m_pipelineState = [m_device newRenderPipelineStateWithDescriptor: m_pipelineStateDesc error: &error];
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
