#include "PreCompile.h"
#include "Pharos.h"

MetalSamplerState::MetalSamplerState()
{
}

MetalSamplerState::~MetalSamplerState(void)
{
}

bool MetalSamplerState::CreateState(const SamplerStateDesc& desc)
{
	return true;
}

RenderSamplerState* MetalSamplerState::Clone()
{
    return NULL;
}

void MetalSamplerState::ApplyDevice(uint32 slot)
{
}
