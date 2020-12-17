#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderer::VulkanRenderer(void)
{
	m_device = VK_NULL_HANDLE;
	
	m_cmdPool = VK_NULL_HANDLE;
	m_cmdBuf = VK_NULL_HANDLE;
	m_descPool = VK_NULL_HANDLE;
	
	m_queue = VK_NULL_HANDLE;
	
	m_swapchain = VK_NULL_HANDLE;

	m_defaultTarget = NULL;
}

VulkanRenderer::~VulkanRenderer(void)
{
}

IMPL_MAKE_RENDERER(VulkanRenderer)

bool VulkanRenderer::Init()
{
	MyWindow* window = (MyWindow*)sKernel->GetWindowHandle();

	if(!sInitHelper->Initialize(window))
	{
		sInitHelper->Destroy();
		return false;
	}	

	m_device = sInitHelper->GetVulkanDevice();
	m_queue = sInitHelper->GetVulkanQueue();

	uint32	queueFamilyIndex = sInitHelper->GetQueueFamilyIndex();
	m_swapchain = sInitHelper->GetSwapchain();
	int32 surfaceWidth = sInitHelper->GetWindowSurfaceWidth();
	int32 surfaceHeight = sInitHelper->GetWindowSurfaceHeight();
	VkFormat surfaceColorFormat = sInitHelper->GetWindowSurfaceColorFormat();
	VkFormat surfaceDepthFormat = sInitHelper->GetWindowSurfaceDepthFormat();

	m_cmdPool = CreateCommandPool(m_device, queueFamilyIndex);
	if(m_cmdPool == VK_NULL_HANDLE) return false;

	m_cmdBuf = CreateCommandBuffer(m_device, m_cmdPool);
	if(m_cmdBuf == VK_NULL_HANDLE) return false;

	m_descPool = CreateDescriptorPool(m_device);
	if (m_descPool == VK_NULL_HANDLE) return false;

	m_defaultTarget = new VulkanDefaultTarget(m_device, surfaceWidth, surfaceHeight, m_swapchain);
	m_defaultTarget->InitDefaultTarget(surfaceColorFormat, surfaceDepthFormat);

	return true;
}

void VulkanRenderer::Destroy()
{
	vkDestroySwapchainKHR(m_device, m_swapchain, NULL);

	vkDestroyDescriptorPool(m_device, m_descPool, NULL);

	vkFreeCommandBuffers(m_device, m_cmdPool, 1, &m_cmdBuf);
	vkDestroyCommandPool(m_device, m_cmdPool, NULL);

	SAFE_DELETE(m_defaultTarget);

	sInitHelper->Destroy();
}

const char8* VulkanRenderer::GetAdapterName() const
{ 
	return sInitHelper->GetDefaultPhysicalDeviceName();
}

uint32 VulkanRenderer::GetAdapterMemorySize() const
{
	 return sInitHelper->GetDefaultPhysicalDeviceMemorySize(); 
}

bool VulkanRenderer::Create(const DeviceConfig& cfg)
{
	m_defaultTarget->SetClear(cfg.backColor);

	return true;
}

void VulkanRenderer::Commit()
{
    VkFence drawFence = m_defaultTarget->GetCurrentFence();
	VkSemaphore semaphore = m_defaultTarget->GetSemaphore();

	const VkCommandBuffer cmd_bufs[] = {m_cmdBuf};
    VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submit_info[1] = {};
    submit_info[0].pNext = NULL;
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[0].waitSemaphoreCount = 1;
    submit_info[0].pWaitSemaphores = &semaphore;
    submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
    submit_info[0].commandBufferCount = 1;
    submit_info[0].pCommandBuffers = cmd_bufs;
    submit_info[0].signalSemaphoreCount = 0;
    submit_info[0].pSignalSemaphores = NULL;

	vkResetFences(m_device, 1, &drawFence);

    VkResult res = vkQueueSubmit(m_queue, 1, submit_info, drawFence);
    assert(res == VK_SUCCESS);

	m_defaultTarget->PresentQueue(m_queue);

	//usleep(1);

	//vkDestroyFence(m_device, drawFence, NULL);
}

RenderBuffer *VulkanRenderer::GenerateRenderBuffer(BufferType type)
{
	return new VulkanRenderBuffer(type, m_device);;
}

RenderTexture* VulkanRenderer::CreateTexture2D(int32 width, int32 height, EPixelFormat fmt)
{
	VulkanRenderTexture* texture = new VulkanRenderTexture(m_device, m_cmdPool, m_queue);
	if (!texture->Create2D(width, height, fmt))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}
	return texture;
}

RenderTexture* VulkanRenderer::CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt)
{
    VulkanRenderTexture* texture = new VulkanRenderTexture(m_device, m_cmdPool, m_queue);

    if (fmt == EPF_D32_FLOAT_S8_UINT || fmt == EPF_D32_FLOAT || fmt == EPF_D24_UNORM_S8_UINT || fmt == EPF_D16_UNORM)
    {
        if (!texture->CreateDepthAttachment(width, height, fmt))
        {
            SAFE_DELETE(texture);
            return nullptr;
        }
    }
    else
    {
        if (!texture->CreateColorAttatchment(width, height, fmt))
        {
            SAFE_DELETE(texture);
            return nullptr;
        }
    }
	
    return texture;
}

RenderTexture* VulkanRenderer::LoadTexture(const char8* szPath)
{
	VulkanRenderTexture* texture = new VulkanRenderTexture(m_device, m_cmdPool, m_queue);
	if (!texture->LoadFromFile(szPath))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}
	return texture;
}

RenderTexture* VulkanRenderer::LoadTexture(const Image* image)
{
	VulkanRenderTexture* texture = new VulkanRenderTexture(m_device, m_cmdPool, m_queue);
	if (!texture->LoadFromImage(image))
	{
		SAFE_DELETE(texture);
		return nullptr;
	}
	return texture;
}

RenderProgram* VulkanRenderer::GenerateRenderProgram()
{
	return new VulkanShaderProgram(m_device);
}

RenderTarget* VulkanRenderer::CreateRenderTarget(int32 width, int32 height)
{	
	return new VulkanRenderTarget(m_device, width, height);;
}

RenderCommand* VulkanRenderer::GenerateRenderCommand()
{
	return new VulkanRenderCommand(m_device, m_cmdBuf);
}

RenderPipeline* VulkanRenderer::GenerateRenderPipeline()
{
	return new VulkanRenderPipeline(m_device);
}

RenderResourceSet* VulkanRenderer::GenerateRenderResuourceSet()
{
	return new VulkanDesciptorSet(m_device, m_descPool);
}

VkDescriptorPool VulkanRenderer::CreateDescriptorPool(VkDevice device)
{
    VkDescriptorPoolSize type_count[2];
    type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    type_count[0].descriptorCount = 1000;
    type_count[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    type_count[1].descriptorCount = 1000;

    VkDescriptorPoolCreateInfo descriptor_pool = {};
    descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool.pNext = NULL;
    descriptor_pool.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    descriptor_pool.maxSets = 1000;
    descriptor_pool.poolSizeCount = 2;
    descriptor_pool.pPoolSizes = type_count;

	VkDescriptorPool descPool;
    VkResult res = vkCreateDescriptorPool(device, &descriptor_pool, NULL, &descPool);
    if (res != VK_SUCCESS) return VK_NULL_HANDLE;

	return descPool;
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
