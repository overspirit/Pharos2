#include "PreCompile.h"
#include "Pharos.h"

VulkanDefaultTarget::VulkanDefaultTarget(VkDevice device, int32 width, int32 height, VkSwapchainKHR swapchain) 
: VulkanRenderTarget(device, width, height)
{	
    m_swapchain = swapchain;

    m_semaphore = NULL;    
    
    m_currFrameIndex = 0;    

    m_swapchainFence = VK_NULL_HANDLE;
}

VulkanDefaultTarget::~VulkanDefaultTarget(void)
{
    SAFE_DELETE(m_depthAttachment);     // default target 要自己删除深度纹理，

    for (VkImageView imageView : m_colorImageViews)
    {
        vkDestroyImageView(m_device, imageView, NULL);
    }    

    vkDestroySemaphore(m_device, m_semaphore, NULL);

    for (VkFramebuffer frameBuf : m_frameBufList)
    {
        vkDestroyFramebuffer(m_device, frameBuf, NULL);
    }
    
    vkDestroyFence(m_device, m_swapchainFence, NULL);
}

void VulkanDefaultTarget::CreateRenderPass(VkFormat colorFormat, VkFormat depthStencilFormat)
{
    VkAttachmentDescription colorAttachment;
    colorAttachment.format = colorFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.flags = 0;
    m_attachments.push_back(colorAttachment);

    VkAttachmentDescription depthStencilAttachment;
    depthStencilAttachment.format = depthStencilFormat;
    depthStencilAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthStencilAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthStencilAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthStencilAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthStencilAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthStencilAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthStencilAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    depthStencilAttachment.flags = 0;
    m_attachments.push_back(depthStencilAttachment);

    VkAttachmentReference colorReference = {};
    colorReference.attachment = 0;
    colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    m_colorReferenceList.push_back(colorReference);

    m_depthReference.attachment = 1;
    m_depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags = 0;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = NULL;
    subpass.colorAttachmentCount = m_colorReferenceList.size();
    subpass.pColorAttachments = m_colorReferenceList.data();
    subpass.pResolveAttachments = NULL;
    subpass.pDepthStencilAttachment = &m_depthReference;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = NULL;

    VkSubpassDependency subpassDependency = {};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDependency.dependencyFlags = 0;

    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = NULL;
    renderPassCreateInfo.attachmentCount = m_attachments.size();
    renderPassCreateInfo.pAttachments = m_attachments.data();
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpass;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = &subpassDependency;

    VkResult res = vkCreateRenderPass(m_device, &renderPassCreateInfo, NULL, &m_renderPass);
    assert (res == VK_SUCCESS);
}

bool VulkanDefaultTarget::CreateColorAttachment(VkFormat colorFormat)
{
    uint32 swapchainImageCount = 0;
    VkResult res = vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchainImageCount, NULL);
    if (res != VK_SUCCESS)
        return false;

    m_colorImages.resize(swapchainImageCount);
    res = vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchainImageCount, m_colorImages.data());
    if (res != VK_SUCCESS)
        return false;

    for (VkImage image : m_colorImages)
    {
        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = NULL;
        imageViewCreateInfo.format = colorFormat;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.image = image;

        VkImageView imageView;
        VkResult res = vkCreateImageView(m_device, &imageViewCreateInfo, NULL, &imageView);
        assert(res == VK_SUCCESS);

        m_colorImageViews.push_back(imageView);
    }

    return true;
}

bool VulkanDefaultTarget::InitDefaultTarget(VkFormat colorFormat, VkFormat depthFormat)
{
    CreateColorAttachment(colorFormat);    

    m_depthAttachment = (VulkanRenderTexture*)sRenderer->CreateTargetTexture(m_width, m_height, VkFormat2PixelFormat(depthFormat));
    VkImageView depthImageView = m_depthAttachment->GetVulkanImageInfo().imageView;

    CreateRenderPass(colorFormat, depthFormat);

    for (VkImageView colorImageView : m_colorImageViews)
    {
        VkImageView attachments[2];
        attachments[0] = colorImageView;
        attachments[1] = depthImageView;

        VkFramebufferCreateInfo frameBufferCreateInfo = {};
        frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferCreateInfo.pNext = NULL;
        frameBufferCreateInfo.renderPass = m_renderPass;
        frameBufferCreateInfo.attachmentCount = 2;
        frameBufferCreateInfo.pAttachments = attachments;
        frameBufferCreateInfo.width = m_width;
        frameBufferCreateInfo.height = m_height;
        frameBufferCreateInfo.layers = 1;

        VkFramebuffer frameBuffer;
        VkResult res = vkCreateFramebuffer(m_device, &frameBufferCreateInfo, NULL, &frameBuffer);
        if(res != VK_SUCCESS) return false;

        m_frameBufList.push_back(frameBuffer);
    }

    VkFenceCreateInfo fenceInfo;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkResult res = vkCreateFence(m_device, &fenceInfo, NULL, &m_swapchainFence);
    if(res != VK_SUCCESS) return false;


	VkSemaphoreCreateInfo semaphoreCreateInfo;
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = NULL;
    semaphoreCreateInfo.flags = 0;	

    res = vkCreateSemaphore(m_device, &semaphoreCreateInfo, NULL, &m_semaphore);
    if(res != VK_SUCCESS) return false;

	return true;
}

VkRenderPassBeginInfo VulkanDefaultTarget::MakeRenderPassBeginInfo()
{
    VkRenderPassBeginInfo rp_begin = {};
    rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

    VkResult res = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_semaphore, VK_NULL_HANDLE, &m_currFrameIndex);
    if (res == VK_ERROR_SURFACE_LOST_KHR)
    {
        //重建swapchain
        //...
        vkDeviceWaitIdle(m_device);

        return rp_begin;
    }

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

    VkFence waitFence = m_swapchainFence;

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

void VulkanDefaultTarget::SetColorAttach(uint32 slot, RenderTexture* tex)
{
}

RenderTexture* VulkanDefaultTarget::GetColorAttachTexture(uint32 slot)
{
	return nullptr;
}

void VulkanDefaultTarget::SetDepthStencilAttach(RenderTexture* tex)
{
}

RenderTexture* VulkanDefaultTarget::GetDepthStencilAttachTexture()
{
	return nullptr;
}