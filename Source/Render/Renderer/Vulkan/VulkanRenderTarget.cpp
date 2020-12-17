#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderTarget::VulkanRenderTarget(VkDevice device, int32 width, int32 height)
{
	m_device = device;

	m_width = width;
	m_height = height;

	m_renderPass = VK_NULL_HANDLE;
    m_frameBuffer = VK_NULL_HANDLE;

	m_clearValues[0].color.float32[0] = 1.0f;
    m_clearValues[0].color.float32[1] = 1.0f;
    m_clearValues[0].color.float32[2] = 1.0f;
    m_clearValues[0].color.float32[3] = 1.0f;
    m_clearValues[1].depthStencil.depth = 1.0f;
    m_clearValues[1].depthStencil.stencil = 0;
	
	m_depthAttachment = NULL;
}

VulkanRenderTarget::~VulkanRenderTarget(void)
{
	vkDestroyRenderPass(m_device, m_renderPass, NULL);

	vkDestroyFramebuffer(m_device, m_frameBuffer, NULL);
}

void VulkanRenderTarget::SetClear(Color4 color, float32 depth, uint32 stencil)
{
	Color4f clearColor = Color4f(color);
	
	m_clearValues[0].color.float32[0] = clearColor.r;
    m_clearValues[0].color.float32[1] = clearColor.g;
    m_clearValues[0].color.float32[2] = clearColor.b;
    m_clearValues[0].color.float32[3] = clearColor.a;
    m_clearValues[1].depthStencil.depth = depth;
    m_clearValues[1].depthStencil.stencil = stencil;
}

void VulkanRenderTarget::SetColorAttach(uint32 slot, RenderTexture* tex)
{
	if (tex == NULL) return;

	VulkanRenderTexture* vulkanTex = static_cast<VulkanRenderTexture*>(tex);

	VkAttachmentReference attachmentReference;
	attachmentReference.attachment = m_attachments.size();
	attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	m_colorReferenceList.push_back(attachmentReference);

	VkAttachmentDescription colorAttachment;
	colorAttachment.format = PixelFormat2VkFormat(vulkanTex->GetFormat());
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	colorAttachment.flags = 0;
	m_attachments.push_back(colorAttachment);

	m_colorAttachmentList.push_back(vulkanTex);
	m_imageViewList.push_back(vulkanTex->GetVulkanImageInfo().imageView);
}

RenderTexture* VulkanRenderTarget::GetColorAttachTexture(uint32 slot)
{
	return m_colorAttachmentList[slot];
}

void VulkanRenderTarget::SetDepthStencilAttach(RenderTexture* tex)
{
	if (tex == NULL) return;
	
	VulkanRenderTexture* vulkanTex = static_cast<VulkanRenderTexture*>(tex);

	m_depthReference.attachment = m_attachments.size();
	m_depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthStencilAttachment;
    depthStencilAttachment.format = PixelFormat2VkFormat(vulkanTex->GetFormat());
    depthStencilAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthStencilAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthStencilAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthStencilAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthStencilAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthStencilAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthStencilAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    depthStencilAttachment.flags = 0;
    m_attachments.push_back(depthStencilAttachment);
	
	m_depthAttachment = vulkanTex;
	m_imageViewList.push_back(vulkanTex->GetVulkanImageInfo().imageView);
}

RenderTexture* VulkanRenderTarget::GetDepthStencilAttachTexture()
{
	return m_depthAttachment;
}

VkRenderPassBeginInfo VulkanRenderTarget::MakeRenderPassBeginInfo()
{
    if (m_renderPass == VK_NULL_HANDLE)
    {
		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount    = m_colorReferenceList.size();
		subpassDescription.pColorAttachments       = m_colorReferenceList.data();
		subpassDescription.pDepthStencilAttachment = (m_depthAttachment != NULL)? &m_depthReference : NULL;

        vector<VkSubpassDependency> subpassDependencies(2);
        subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependencies[0].dstSubpass = 0;
        subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        subpassDependencies[1].srcSubpass = 0;
        subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        subpassDependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.pNext = NULL;
        renderPassInfo.attachmentCount = m_attachments.size();
        renderPassInfo.pAttachments = m_attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpassDescription;
        renderPassInfo.dependencyCount = subpassDependencies.size();
        renderPassInfo.pDependencies = subpassDependencies.data();

        VkResult res = vkCreateRenderPass(m_device, &renderPassInfo, NULL, &m_renderPass);
        assert(res == VK_SUCCESS);
	}

	if (m_frameBuffer == NULL)
	{
		VkFramebufferCreateInfo frameBufferInfo;
		frameBufferInfo.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.pNext			= NULL;
		frameBufferInfo.renderPass      = m_renderPass;
		frameBufferInfo.attachmentCount = m_imageViewList.size();
		frameBufferInfo.pAttachments    = m_imageViewList.data();
		frameBufferInfo.width           = m_width;
		frameBufferInfo.height          = m_height;
		frameBufferInfo.layers          = 1;
		VkResult res = vkCreateFramebuffer(m_device, &frameBufferInfo, NULL, &m_frameBuffer);
		assert(res == VK_SUCCESS);
	}

    VkRenderPassBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.pNext = NULL;
    beginInfo.renderPass = m_renderPass;
    beginInfo.framebuffer = m_frameBuffer;
    beginInfo.renderArea.offset.x = 0;
    beginInfo.renderArea.offset.y = 0;
    beginInfo.renderArea.extent.width = m_width;
    beginInfo.renderArea.extent.height = m_height;
    beginInfo.clearValueCount = 2;
    beginInfo.pClearValues = m_clearValues; 

	return beginInfo;
}