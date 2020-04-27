#include "PreCompile.h"
#include "Pharos.h"

MetalShaderProgram::MetalShaderProgram()
{
}

MetalShaderProgram::~MetalShaderProgram()
{
}

bool MetalShaderProgram::CompileVertexShader(const char8* shader, const char8* enter)
{
	//    NSError* error = nil;
	//    
	//    File file;
	//    string shaderPath = sKernel->GetBundleDirectoryPath() + string("Shader/Shaders.metal");
	//    file.Open(shaderPath.c_str());
	//    uint32 fileSize = file.GetSize();
	//    char* shaderText = new char[fileSize];
	//    file.Read(shaderText, fileSize);
	//    NSString* shaderSource = [NSString stringWithUTF8String:shaderText];
	//    
	//    id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
	//    
	//    id <MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
	//    
	//    id <MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader"];

	MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
	//    pipelineStateDescriptor.label = @"MyPipeline";
	//    pipelineStateDescriptor.sampleCount = view.sampleCount;
	//    pipelineStateDescriptor.vertexFunction = vertexFunction;
	//    pipelineStateDescriptor.fragmentFunction = fragmentFunction;
	//    pipelineStateDescriptor.vertexDescriptor = _mtlVertexDescriptor;
	//    pipelineStateDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat;
	//    pipelineStateDescriptor.depthAttachmentPixelFormat = view.depthStencilPixelFormat;
	//    pipelineStateDescriptor.stencilAttachmentPixelFormat = view.depthStencilPixelFormat;
	//
	//    //NSError *error = NULL;
	//    _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
	//    if (!_pipelineState)
	//    {
	//        NSLog(@"Failed to created pipeline state, error %@", error);
	//    }

	return true;
}

bool MetalShaderProgram::CompilePixelShader(const char8* shader, const char8* enter)
{
	return true;
}

//RenderProgram* MetalShaderProgram::Clone()
//{
//    return NULL;
//}

void MetalShaderProgram::ApplyDevice()
{
}
