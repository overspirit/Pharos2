#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderTarget::VulkanRenderTarget(VkDevice device)
{
	m_device = device;
}

VulkanRenderTarget::~VulkanRenderTarget(void)
{
}

bool VulkanRenderTarget::Init(int32 width, int32 height)
{
	return true;
}

bool VulkanRenderTarget::CreateDefaultFrameBuffer(VkSwapchainKHR swapchain, VkSemaphore semaphore, int32 width, int32 height, VkFormat format)
{
	VkResult res;

	m_swapchain = swapchain;
	m_semaphore = semaphore;
	m_width = width;
	m_height = height;

	 m_renderPass = CreateRenderPass(m_device, format);
	 if(m_renderPass == VK_NULL_HANDLE) return false;

	uint32 swapchainImageCount = 0;
	res = vkGetSwapchainImagesKHR(m_device, swapchain, &swapchainImageCount, NULL);
    if(res != VK_SUCCESS) return false;

	m_swapchainImages.resize(swapchainImageCount);
    res = vkGetSwapchainImagesKHR(m_device, swapchain, &swapchainImageCount, m_swapchainImages.data());
    if(res != VK_SUCCESS) return false;

	m_swapchainImageViews = CreateSurfaceImageViews(m_device, format, m_swapchainImages);
	if(m_swapchainImageViews.size() != swapchainImageCount) return false;

	m_frameBufList.resize(swapchainImageCount);
	
	VkImageView attachments[2];
    //attachments[1] = info.depth.view;

    VkFramebufferCreateInfo fb_info = {};
    fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fb_info.pNext = NULL;
    fb_info.renderPass = m_renderPass;
    fb_info.attachmentCount = 1;//include_depth ? 2 : 1;
    fb_info.pAttachments = attachments;
    fb_info.width = m_width;
    fb_info.height = m_height;
    fb_info.layers = 1;

    for (uint32 i = 0; i < swapchainImageCount; i++)
	{
        attachments[0] = m_swapchainImageViews[i];
        VkResult res = vkCreateFramebuffer(m_device, &fb_info, NULL, &m_frameBufList[i]);
        if(res != VK_SUCCESS) return false;
    }

	return true;
}

#define NUM_SAMPLES VK_SAMPLE_COUNT_1_BIT

VkRenderPass VulkanRenderTarget::CreateRenderPass(VkDevice device, VkFormat format)
{
	VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VkAttachmentDescription attachments[2];
	attachments[0].format = format;
	attachments[0].samples = NUM_SAMPLES;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = initialLayout;
	attachments[0].finalLayout = finalLayout;
	attachments[0].flags = 0;

	// attachments[1].format = depth.format;
	// attachments[1].samples = NUM_SAMPLES;
	// attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	// attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	// attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	// attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
	// attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	// attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	// attachments[1].flags = 0;

	VkAttachmentReference color_reference = {};
	color_reference.attachment = 0;
	color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// VkAttachmentReference depth_reference = {};
	// depth_reference.attachment = 1;
	// depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.flags = 0;
	subpass.inputAttachmentCount = 0;
	subpass.pInputAttachments = NULL;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_reference;
	subpass.pResolveAttachments = NULL;
	subpass.pDepthStencilAttachment = NULL;//&depth_reference;
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments = NULL;

	VkSubpassDependency subpass_dependency = {};
	subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpass_dependency.dstSubpass = 0;
	subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpass_dependency.srcAccessMask = 0;
	subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	subpass_dependency.dependencyFlags = 0;

	VkRenderPassCreateInfo rp_info = {};
	rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rp_info.pNext = NULL;
	rp_info.attachmentCount = 1;//2;
	rp_info.pAttachments = attachments;
	rp_info.subpassCount = 1;
	rp_info.pSubpasses = &subpass;
	rp_info.dependencyCount = 1;
	rp_info.pDependencies = &subpass_dependency;

	VkRenderPass renderPass;
	VkResult res = vkCreateRenderPass(device, &rp_info, NULL, &renderPass);
	if(res != VK_SUCCESS) return VK_NULL_HANDLE;

	return renderPass;
}

std::vector<VkImageView> VulkanRenderTarget::CreateSurfaceImageViews(VkDevice device, VkFormat surfaceFormat, const vector<VkImage> surfaceImages)
{
	std::vector<VkImageView> imageViews;

	for (uint32_t i = 0; i < surfaceImages.size(); i++) 
	{
        VkImageViewCreateInfo color_image_view = {};
        color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        color_image_view.pNext = NULL;
        color_image_view.format = surfaceFormat;
        color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;
        color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;
        color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;
        color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;
        color_image_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        color_image_view.subresourceRange.baseMipLevel = 0;
        color_image_view.subresourceRange.levelCount = 1;
        color_image_view.subresourceRange.baseArrayLayer = 0;
        color_image_view.subresourceRange.layerCount = 1;
        color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
        color_image_view.flags = 0;
        color_image_view.image = surfaceImages[i];

		VkImageView imageView;
        VkResult res = vkCreateImageView(device, &color_image_view, NULL, &imageView);
        if(res != VK_SUCCESS)
		{
			return imageViews;
		}

		imageViews.push_back(imageView);
    }

	return imageViews;
}

VkRenderPassBeginInfo VulkanRenderTarget::GetRenderPassBeginInfo()
{
    VkResult res = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_semaphore, VK_NULL_HANDLE, &m_currFrameIndex);
    assert(res == VK_SUCCESS);

    m_clearValues[0].color.float32[0] = 0.5f;
    m_clearValues[0].color.float32[1] = 0.5f;
    m_clearValues[0].color.float32[2] = 0.5f;
    m_clearValues[0].color.float32[3] = 0.5f;
    m_clearValues[1].depthStencil.depth = 1.0f;
    m_clearValues[1].depthStencil.stencil = 0;

    VkRenderPassBeginInfo rp_begin;
    rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin.pNext = NULL;
    rp_begin.renderPass = m_renderPass;
    rp_begin.framebuffer = m_frameBufList[m_currFrameIndex];
    rp_begin.renderArea.offset.x = 0;
    rp_begin.renderArea.offset.y = 0;
    rp_begin.renderArea.extent.width = m_width;
    rp_begin.renderArea.extent.height = m_height;
    rp_begin.clearValueCount = 1;
    rp_begin.pClearValues = m_clearValues; 

	return rp_begin;
}

void VulkanRenderTarget::PresentQueue(VkQueue queue)
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

    VkResult res = vkQueuePresentKHR(queue, &presentInfo);
    assert(res == VK_SUCCESS);
}

void VulkanRenderTarget::SetClear(Color4 color, float32 depth, uint32 stencil)
{
}

RenderTexture *VulkanRenderTarget::GenerateColorAttach(uint32 slot, EPixelFormat fmt)
{
	return nullptr;
}

void VulkanRenderTarget::SetColorAttach(uint32 slot, RenderTexture *tex)
{
}

RenderTexture *VulkanRenderTarget::GenerateDepthAttach(EPixelFormat fmt)
{
	return nullptr;
}

void VulkanRenderTarget::SetDepthAttach(RenderTexture *tex)
{
}

RenderTexture *VulkanRenderTarget::GenerateStencilAttach(EPixelFormat fmt)
{
	return nullptr;
}

void VulkanRenderTarget::SetStencilAttach(RenderTexture *tex)
{
}

RenderTexture *VulkanRenderTarget::GetColorAttachTexture(uint32 slot)
{
	return nullptr;
}

RenderTexture *VulkanRenderTarget::GetDepthAttachTexture()
{
	return nullptr;
}

RenderTexture *VulkanRenderTarget::GetStencilAttachTexture()
{
	return nullptr;
}

EPixelFormat VulkanRenderTarget::GetColorAttachFormat(uint32 slot)
{
	return EPF_END;
}

EPixelFormat VulkanRenderTarget::GetDepthAttachFormat()
{
	return EPF_END;
}

EPixelFormat VulkanRenderTarget::GetStencilAttachFormat()
{
	return EPF_END;
}
