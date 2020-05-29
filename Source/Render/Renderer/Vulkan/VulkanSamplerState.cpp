#include "PreCompile.h"
#include "Pharos.h"

VulkanSamplerState::VulkanSamplerState()
{
}

VulkanSamplerState::~VulkanSamplerState(void)
{
}

bool VulkanSamplerState::CreateState(const SamplerStateDesc& desc)
{
	return true;
}

RenderSamplerState* VulkanSamplerState::Clone()
{
    return NULL;
}

void VulkanSamplerState::ApplyDevice(uint32 slot)
{
}
