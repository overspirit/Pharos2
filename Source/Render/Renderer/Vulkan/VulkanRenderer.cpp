#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderer::VulkanRenderer(void)
{
}

VulkanRenderer::~VulkanRenderer(void)
{
}

bool VulkanRenderer::Init()
{
	glfw_window* window = (glfw_window*)sKernel->GetWindowHandle();

	if(!sInitHelper->Initialize(window))
	{
		sInitHelper->Destroy();
		return false;
	}	

	m_device = sInitHelper->GetVulkanDevice();
	m_queue = sInitHelper->GetVulkanQueue();

	uint32	queueFamilyIndex = sInitHelper->GetQueueFamilyIndex();
	VkSwapchainKHR swapchain = sInitHelper->GetSwapchain();
	int32 surfaceWidth = sInitHelper->GetWindowSurfaceWidth();
	int32 surfaceHeight = sInitHelper->GetWindowSurfaceHeight();
	VkFormat surfaceFormat = sInitHelper->GetWindowSurfaceFormat();

	m_cmdPool = CreateCommandPool(m_device, queueFamilyIndex);
	if(m_cmdPool == VK_NULL_HANDLE) return false;

	m_cmdBuf = CreateCommandBuffer(m_device, m_cmdPool);
	if(m_cmdBuf == VK_NULL_HANDLE) return false;

	m_semaphore = CreateSemaphore(m_device);
	if(m_semaphore == VK_NULL_HANDLE) return false;

	m_defaultTarget = new VulkanRenderTarget(m_device);
	m_defaultTarget->CreateDefaultFrameBuffer(swapchain, m_semaphore, surfaceWidth, surfaceHeight, surfaceFormat);

	return true;
}

void VulkanRenderer::Destroy()
{

}

bool VulkanRenderer::Create(const DeviceConfig& cfg)
{
	return true;
}

void VulkanRenderer::Commit()
{
	VkFence drawFence = CreateDrawFence(m_device);

	const VkCommandBuffer cmd_bufs[] = {m_cmdBuf};
    VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submit_info[1] = {};
    submit_info[0].pNext = NULL;
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[0].waitSemaphoreCount = 1;
    submit_info[0].pWaitSemaphores = &m_semaphore;
    submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
    submit_info[0].commandBufferCount = 1;
    submit_info[0].pCommandBuffers = cmd_bufs;
    submit_info[0].signalSemaphoreCount = 0;
    submit_info[0].pSignalSemaphores = NULL;

    VkResult res = vkQueueSubmit(m_queue, 1, submit_info, drawFence);
    assert(res == VK_SUCCESS);

	do
	{
		res = vkWaitForFences(m_device, 1, &drawFence, VK_TRUE, 100000000);
	} while (res == VK_TIMEOUT);
	assert(res == VK_SUCCESS);

	m_defaultTarget->PresentQueue(m_queue);

	vkDestroyFence(m_device, drawFence, NULL);
}

//
//RenderProgram* VulkanRenderer::GenerateRenderProgram()
//{
//    return new VulkanShaderProgram();
//}
//
RenderBuffer *VulkanRenderer::GenerateRenderBuffer(BufferType type)
{
	if (type == BufferType::UNIFORM_BUFFFER)
	{
		return new VulkanUniformBuffer(type, m_device);
	}
	else
	{
		return new VulkanRenderBuffer(type, m_device);
	}

	return nullptr;
}

RenderTexture* VulkanRenderer::CreateTexture(int32 width, int32 height, EPixelFormat fmt)
{
	return nullptr;
}
//
//RenderTexture* VulkanRenderer::CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt)
//{
//    VulkanTexture* texture = new VulkanTexture();
//    if (!texture->CreateTargetTexture(width, height, fmt))
//    {
//        SAFE_DELETE(texture);
//        return nullptr;
//    }
//    
//    return texture;
//}
//
//RenderTexture* VulkanRenderer::CreateDepthTexture(int32 width, int32 height)
//{
//    VulkanTexture* texture = new VulkanTexture();
//    if (!texture->CreateDepthTexture(width, height))
//    {
//        SAFE_DELETE(texture);
//        return nullptr;
//    }
//    
//    return texture;
//}

RenderTexture* VulkanRenderer::LoadTexture(const char8* szPath)
{
	return nullptr;
}

RenderTexture* VulkanRenderer::LoadTexture(const Image* image)
{
	return nullptr;
}

RenderProgram* VulkanRenderer::GenerateRenderProgram()
{
	return new VulkanShaderProgram(m_device);
}

RenderTarget* VulkanRenderer::CreateRenderTarget(int32 width, int32 height)
{
	//    VulkanFrameBuffer* frameBuf = new VulkanFrameBuffer();
	//    if (!frameBuf->InitFrameBuffer(width, height))
	//    {
	//        SAFE_DELETE(frameBuf);
	//        return nullptr;
	//    }
	//
	//    return frameBuf;

	return nullptr;
}

RenderSamplerState* VulkanRenderer::CreateSampleState(const SamplerStateDesc& desc)
{
	return nullptr;
}

RenderBlendState* VulkanRenderer::CreateBlendState(const BlendStateDesc& desc)
{
	return nullptr;
}

RenderRasterizerState* VulkanRenderer::CreateRasterizerState(const RasterizerStateDesc& desc)
{
	return nullptr;
}

RenderDepthStencilState* VulkanRenderer::CreateDepthStencilState(const DepthStencilStateDesc& desc)
{
	return nullptr;
}

RenderCommand* VulkanRenderer::GenerateRenderCommand(RenderTarget* renderTarget)
{
	return new VulkanRenderCommand(m_device, m_cmdBuf, static_cast<VulkanRenderTarget*>(renderTarget));
}

RenderPipeline* VulkanRenderer::GenerateRenderPipeline()
{
	return new VulkanRenderPipeline(m_device);
}

VkCommandPool VulkanRenderer::CreateCommandPool(VkDevice device, uint32 queueFamilyIndex)
{
	VkCommandPoolCreateInfo cmd_pool_info = {};
    cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_info.pNext = NULL;
    cmd_pool_info.queueFamilyIndex = queueFamilyIndex;
    cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VkCommandPool cmd_pool;
    VkResult res = vkCreateCommandPool(device, &cmd_pool_info, NULL, &cmd_pool);
    if(res != VK_SUCCESS) return VK_NULL_HANDLE;

	return cmd_pool;
}

VkCommandBuffer VulkanRenderer::CreateCommandBuffer(VkDevice device, VkCommandPool cmdPool)
{
	VkCommandBufferAllocateInfo cmdBufInfo = {};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufInfo.pNext = NULL;
    cmdBufInfo.commandPool = cmdPool;
    cmdBufInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufInfo.commandBufferCount = 1;

	VkCommandBuffer cmdBuf;
    VkResult res = vkAllocateCommandBuffers(device, &cmdBufInfo, &cmdBuf);
    if(res != VK_SUCCESS) return VK_NULL_HANDLE;

	return cmdBuf;
}

VkSemaphore VulkanRenderer::CreateSemaphore(VkDevice device)
{
	VkSemaphoreCreateInfo semaphoreCreateInfo;
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = NULL;
    semaphoreCreateInfo.flags = 0;	

	VkSemaphore semaphore;
    VkResult res = vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &semaphore);
    if(res != VK_SUCCESS) return VK_NULL_HANDLE;

	return semaphore;
}

VkFence VulkanRenderer::CreateDrawFence(VkDevice device)
{
    VkFenceCreateInfo fenceInfo;    
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = 0;
	
	VkFence fence;
    VkResult res = vkCreateFence(device, &fenceInfo, NULL, &fence);
	if(res != VK_SUCCESS) return VK_NULL_HANDLE;

	return fence;
}
