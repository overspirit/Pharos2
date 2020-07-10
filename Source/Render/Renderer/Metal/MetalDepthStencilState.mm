#include "PreCompile.h"
#include "Pharos.h"

MetalDepthStencilState::MetalDepthStencilState(id<MTLDevice> device)
{
	m_device = device;
}

MetalDepthStencilState::~MetalDepthStencilState(void)
{
}

bool MetalDepthStencilState::CreateState(const DepthStencilStateDesc& desc)
{
	MTLDepthStencilDescriptor *depthStateDesc = [[MTLDepthStencilDescriptor alloc] init];
	depthStateDesc.depthCompareFunction = MTLCompareFunctionLess;
	depthStateDesc.depthWriteEnabled = YES;
	
	m_depthState = [m_device newDepthStencilStateWithDescriptor:depthStateDesc];
	
	return true;
}

RenderDepthStencilState* MetalDepthStencilState::Clone()
{
	return NULL;
}

void MetalDepthStencilState::ApplyDevice(id <MTLRenderCommandEncoder> renderEncoder)
{
	[renderEncoder setDepthStencilState:m_depthState];
}
