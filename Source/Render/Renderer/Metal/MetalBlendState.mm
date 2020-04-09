#include "PreCompile.h"
#include "Pharos.h"

MetalBlendState::MetalBlendState()
{
}

MetalBlendState::~MetalBlendState(void)
{
}

bool MetalBlendState::CreateState(const BlendStateDesc& desc)
{
	return true;
}

RenderBlendState* MetalBlendState::Clone()
{
    return NULL;
}

void MetalBlendState::ApplyDevice()
{
}
