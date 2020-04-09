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

	return true;
}

RenderDepthStencilState* MetalDepthStencilState::Clone()
{
    return NULL;
}

void MetalDepthStencilState::ApplyDevice()
{
}
