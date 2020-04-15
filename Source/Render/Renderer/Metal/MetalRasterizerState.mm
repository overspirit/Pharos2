#include "PreCompile.h"
#include "Pharos.h"

MetalRasterizerState::MetalRasterizerState()
{
}

MetalRasterizerState:: ~MetalRasterizerState(void)
{
}

bool MetalRasterizerState::CreateState(const RasterizerStateDesc& desc)
{
	return true;
}

RenderRasterizerState* MetalRasterizerState::Clone()
{
    return NULL;
}

void MetalRasterizerState::ApplyDevice()
{
}
