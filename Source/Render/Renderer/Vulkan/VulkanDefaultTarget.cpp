#include "PreCompile.h"
#include "Pharos.h"

VulkanDefaultTarget::VulkanDefaultTarget(VkDevice device, VkSemaphore semaphore) : VulkanRenderTarget(device)
{
	m_semaphore = semaphore;

    m_clearColor = Color4f(1.0f, 1.0f, 1.0f, 1.0f);
    m_clearDepth = 1.0f;
    m_clearStencil = 0;
}

VulkanDefaultTarget::~VulkanDefaultTarget(void)
{
}

bool VulkanDefaultTarget::Init(int32 width, int32 height)
{
	return true;
}

bool VulkanDefaultTarget::CreateDefaultTarget(VkSwapchainKHR swapchain, int32 width, int32 height, VkFormat colorFormat, VkFormat depthFormat)
{	
	m_renderPass = CreateRenderPass(m_device, colorFormat, depthFormat);
	if(m_renderPass == VK_NULL_HANDLE) return false;

	uint32 swapchainImageCount = 0;
	VkResult res = vkGetSwapchainImagesKHR(m_device, swapchain, &swapchainImageCount, NULL);
    if(res != VK_SUCCESS) return false;

	m_swapchainImages.resize(swapchainImageCount);
    res = vkGetSwapchainImagesKHR(m_device, swapchain, &swapchainImageCount, m_swapchainImages.data());
    if(res != VK_SUCCESS) return false;

    VkImage depthImage = CreateDepthImage(width, height, depthFormat);
    VkImageView depthImageView = CreateSurfaceImageViews(m_device, depthFormat, depthImage, VK_IMAGE_ASPECT_DEPTH_BIT);

    for (VkImage image : m_swapchainImages)
    {
	    VkImageView colorImageView = CreateSurfaceImageViews(m_device, colorFormat, image, VK_IMAGE_ASPECT_COLOR_BIT);
        if(colorImageView == VK_NULL_HANDLE) return false;        
	
        VkImageView attachments[2];
        attachments[0] = colorImageView;
        attachments[1] = depthImageView;

        VkFramebufferCreateInfo fb_info = {};
        fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fb_info.pNext = NULL;
        fb_info.renderPass = m_renderPass;
        fb_info.attachmentCount = 2;
        fb_info.pAttachments = attachments;
        fb_info.width = width;
        fb_info.height = height;
        fb_info.layers = 1;

        VkFramebuffer frameBuffer;
        res = vkCreateFramebuffer(m_device, &fb_info, NULL, &frameBuffer);
        if(res != VK_SUCCESS) return false;

        m_swapchainImageViews.push_back(colorImageView);
        m_frameBufList.push_back(frameBuffer);

        VkFence fence = CreateDrawFence(m_device);
        m_swapchainFence.push_back(fence);
    }

    m_swapchainImages.push_back(depthImage);
    m_swapchainImageViews.push_back(depthImageView);

    m_swapchain = swapchain;
	m_width = width;
	m_height = height; 

	return true;
}

VkImage VulkanDefaultTarget::CreateDepthImage(int width, int height, VkFormat depth_format)
{
    const static VkSampleCountFlagBits NUM_SAMPLES = VK_SAMPLE_COUNT_1_BIT;

    VkImageCreateInfo image_info = {};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.pNext = NULL;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.format = depth_format;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.samples = NUM_SAMPLES;
    image_info.tiling = sInitHelper->GetDepthImageTiling(depth_format);
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.queueFamilyIndexCount = 0;
    image_info.pQueueFamilyIndices = NULL;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    image_info.flags = 0;    

    VkImage depthImage;
    VkResult res = vkCreateImage(m_device, &image_info, NULL, &depthImage);
    if(res != VK_SUCCESS) return VK_NULL_HANDLE;    

    VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, depthImage, &mem_reqs);

    VkMemoryAllocateInfo mem_alloc = {};
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc.pNext = NULL;
    mem_alloc.allocationSize = 0;
    mem_alloc.memoryTypeIndex = 0;
    mem_alloc.allocationSize = mem_reqs.size;
    mem_alloc.memoryTypeIndex = sInitHelper->GetMemoryTypeIndex(mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT); 

    VkDeviceMemory imageMemory;
    res = vkAllocateMemory(m_device, &mem_alloc, NULL, &imageMemory);
    if(res != VK_SUCCESS) return VK_NULL_HANDLE;

    res = vkBindImageMemory(m_device, depthImage, imageMemory, 0);
    if(res != VK_SUCCESS) return VK_NULL_HANDLE;

    return depthImage;
}

VkFence VulkanDefaultTarget::CreateDrawFence(VkDevice device)
{
    VkFenceCreateInfo fenceInfo;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkFence fence;
    VkResult res = vkCreateFence(device, &fenceInfo, NULL, &fence);
    if(res != VK_SUCCESS) return VK_NULL_HANDLE;

    return fence;
}

VkRenderPassBeginInfo VulkanDefaultTarget::GetRenderPassBeginInfo()
{
    VkResult res = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_semaphore, VK_NULL_HANDLE, &m_currFrameIndex);
    assert(res == VK_SUCCESS);

    m_clearValues[0].color.float32[0] = m_clearColor.r;
    m_clearValues[0].color.float32[1] = m_clearColor.g;
    m_clearValues[0].color.float32[2] = m_clearColor.b;
    m_clearValues[0].color.float32[3] = m_clearColor.a;
    m_clearValues[1].depthStencil.depth = m_clearDepth;
    m_clearValues[1].depthStencil.stencil = m_clearStencil;

    VkRenderPassBeginInfo rp_begin;
    rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin.pNext = NULL;
    rp_begin.renderPass = m_renderPass;
    rp_begin.framebuffer = m_frameBufList[m_currFrameIndex];
    rp_begin.renderArea.offset.x = 0;
    rp_begin.renderArea.offset.y = 0;
    rp_begin.renderArea.extent.width = m_width;
    rp_begin.renderArea.extent.height = m_height;
    rp_begin.clearValueCount = 2;
    rp_begin.pClearValues = m_clearValues; 

	return rp_begin;
}

void VulkanDefaultTarget::PresentQueue(VkQueue queue)
{
    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = NULL;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapchain;
    presentInfo.pImageIndices = &m_currFrameIndex;
    presentInfo.pWaitSemaphores = NULL;
    presentInfo.waitSemaphoreCount = 0;
    presentInfo.pResults = NULL;

    VkFence waitFence = m_swapchainFence[0];

    VkResult res = VK_NOT_READY;
    do
    {
        res = vkGetFenceStatus(m_device, waitFence);
    } while (res != VK_SUCCESS);

    res = vkWaitForFences(m_device, 1, &waitFence, VK_TRUE, UINT64_MAX);
    vkResetFences(m_device, 1, &waitFence);

    res = vkQueuePresentKHR(queue, &presentInfo);
    assert((res == VK_SUCCESS) || (res == VK_SUBOPTIMAL_KHR)); //VK_SUBOPTIMAL_KHR 可能是横纵向反了。。
}

void VulkanDefaultTarget::SetClear(Color4 color, float32 depth, uint32 stencil)
{
    m_clearColor = Color4f(color);
    m_clearDepth = depth;
    m_clearStencil = stencil;
}

RenderTexture *VulkanDefaultTarget::GenerateColorAttach(uint32 slot, EPixelFormat fmt)
{
	return nullptr;
}

void VulkanDefaultTarget::SetColorAttach(uint32 slot, RenderTexture *tex)
{
}

RenderTexture *VulkanDefaultTarget::GenerateDepthAttach(EPixelFormat fmt)
{
	return nullptr;
}

void VulkanDefaultTarget::SetDepthAttach(RenderTexture *tex)
{
}

RenderTexture *VulkanDefaultTarget::GenerateStencilAttach(EPixelFormat fmt)
{
	return nullptr;
}

void VulkanDefaultTarget::SetStencilAttach(RenderTexture *tex)
{
}

RenderTexture *VulkanDefaultTarget::GetColorAttachTexture(uint32 slot)
{
	return nullptr;
}

RenderTexture *VulkanDefaultTarget::GetDepthAttachTexture()
{
	return nullptr;
}

RenderTexture *VulkanDefaultTarget::GetStencilAttachTexture()
{
	return nullptr;
}

EPixelFormat VulkanDefaultTarget::GetColorAttachFormat(uint32 slot)
{
	return EPF_END;
}

EPixelFormat VulkanDefaultTarget::GetDepthAttachFormat()
{
	return EPF_END;
}

EPixelFormat VulkanDefaultTarget::GetStencilAttachFormat()
{
	return EPF_END;
}
