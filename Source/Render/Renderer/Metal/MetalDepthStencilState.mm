#include "PreCompile.h"
#include "Pharos.h"

MetalDepthStencilState::MetalDepthStencilState()
{
}

MetalDepthStencilState::~MetalDepthStencilState(void)
{
}

bool MetalDepthStencilState::CreateState(const DepthStencilStateDesc& desc)
{
	//    MTLDepthStencilDescriptor *depthStateDesc = [[MTLDepthStencilDescriptor alloc] init];
	//    depthStateDesc.depthCompareFunction = MTLCompareFunctionLess;
	//    depthStateDesc.depthWriteEnabled = YES;
	//    _depthState = [_device newDepthStencilStateWithDescriptor:depthStateDesc];
	
	return true;
}

RenderDepthStencilState* MetalDepthStencilState::Clone()
{
	return NULL;
}

void MetalDepthStencilState::ApplyDevice()
{
}
