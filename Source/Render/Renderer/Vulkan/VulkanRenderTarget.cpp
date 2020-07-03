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



VkRenderPass VulkanRenderTarget::CreateRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthStencilFormat)
{
	const static VkSampleCountFlagBits NUM_SAMPLES = VK_SAMPLE_COUNT_1_BIT;

	VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	vector<VkAttachmentDescription> attachments;

	VkAttachmentDescription colorAttachment;
	colorAttachment.format = colorFormat;
	colorAttachment.samples = NUM_SAMPLES;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = initialLayout;
	colorAttachment.finalLayout = finalLayout;
	colorAttachment.flags = 0;
	attachments.push_back(colorAttachment);

	if (depthStencilFormat != VK_FORMAT_MAX_ENUM)
	{
		VkAttachmentDescription depthStencilAttachment; 
		depthStencilAttachment.format = depthStencilFormat;
		depthStencilAttachment.samples = NUM_SAMPLES;
		depthStencilAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthStencilAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		depthStencilAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthStencilAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
		depthStencilAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthStencilAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		depthStencilAttachment.flags = 0;
		attachments.push_back(depthStencilAttachment);
	}

	VkAttachmentReference color_reference = {};
	color_reference.attachment = 0;
	color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depth_reference = {};
	depth_reference.attachment = 1;
	depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.flags = 0;
	subpass.inputAttachmentCount = 0;
	subpass.pInputAttachments = NULL;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_reference;
	subpass.pResolveAttachments = NULL;
	subpass.pDepthStencilAttachment = (depthStencilFormat != VK_FORMAT_MAX_ENUM) ? &depth_reference : nullptr;
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
	rp_info.attachmentCount = attachments.size();
	rp_info.pAttachments = attachments.data();
	rp_info.subpassCount = 1;
	rp_info.pSubpasses = &subpass;
	rp_info.dependencyCount = 1;
	rp_info.pDependencies = &subpass_dependency;

	VkRenderPass renderPass;
	VkResult res = vkCreateRenderPass(device, &rp_info, NULL, &renderPass);
	if(res != VK_SUCCESS) return VK_NULL_HANDLE;

	return renderPass;
}

VkImageView VulkanRenderTarget::CreateSurfaceImageViews(VkDevice device, VkFormat format, VkImage image, VkImageAspectFlags flag)
{
    VkImageViewCreateInfo color_image_view = {};
    color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    color_image_view.pNext = NULL;
    color_image_view.format = format;
    color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;
    color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;
    color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;
    color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;
    color_image_view.subresourceRange.aspectMask = flag;
    color_image_view.subresourceRange.baseMipLevel = 0;
    color_image_view.subresourceRange.levelCount = 1;
    color_image_view.subresourceRange.baseArrayLayer = 0;
    color_image_view.subresourceRange.layerCount = 1;
    color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
    color_image_view.flags = 0;
    color_image_view.image = image;

    VkImageView imageView;
    VkResult res = vkCreateImageView(device, &color_image_view, NULL, &imageView);
    if (res != VK_SUCCESS)
    {
        return VK_NULL_HANDLE;
    }

    return imageView;
}

VkRenderPassBeginInfo VulkanRenderTarget::GetRenderPassBeginInfo()
{
    VkRenderPassBeginInfo rp_begin;
    rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin.pNext = NULL;
    rp_begin.renderPass = m_renderPass;
    rp_begin.framebuffer = NULL;
    rp_begin.renderArea.offset.x = 0;
    rp_begin.renderArea.offset.y = 0;
    rp_begin.renderArea.extent.width = m_width;
    rp_begin.renderArea.extent.height = m_height;
    rp_begin.clearValueCount = 1;
    rp_begin.pClearValues = m_clearValues; 

	return rp_begin;
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
