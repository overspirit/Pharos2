#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderTexture::VulkanRenderTexture(VkDevice device, VkCommandPool cmdPool, VkQueue queue)
{
	m_device = device;
    m_cmdPool = cmdPool;
    m_queue = queue;

    m_mipLevels = 1;
	
	m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	m_imageInfo.imageView = VK_NULL_HANDLE;
	m_imageInfo.sampler = VK_NULL_HANDLE;
	
	m_image = VK_NULL_HANDLE;
	m_imageView = VK_NULL_HANDLE;
	m_sampler = VK_NULL_HANDLE;
	
	m_stagingBuffer = VK_NULL_HANDLE;
	m_stagingMemory = VK_NULL_HANDLE;
	m_isNeedUpdate = false;
	
	m_deviceMemory = VK_NULL_HANDLE;
}

VulkanRenderTexture::~VulkanRenderTexture()
{
	vkDestroyImage(m_device, m_image, NULL);
	vkDestroyImageView(m_device, m_imageView, NULL);

	vkDestroyBuffer(m_device, m_stagingBuffer, NULL);

	vkFreeMemory(m_device, m_stagingMemory, NULL);
	vkFreeMemory(m_device, m_deviceMemory, NULL);

	vkDestroySampler(m_device, m_sampler, NULL);
}

bool VulkanRenderTexture::LoadFromFile(const char8* szPath)
{
	if (szPath == nullptr || *szPath == '\0') return false;

	Image* image = sResMgr->GenerateImage(szPath);
	if (image == nullptr) return false;

	if (image->GetBPP() != 32)
	{
		if (!image->ConvertTo32Bits()) return false;
	}

	return LoadFromImage(image);
}

bool VulkanRenderTexture::LoadFromImage(const Image* image)
{
    m_width = image->GetWidth();
    m_height = image->GetHeight();
	m_fmt = EPF_BGRA8_UNORM;
    m_eleSize = GetPFSize(m_fmt);

	if (!CreateVulkanImage(VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)) return false;
	if (!CreateVulkanSampler()) return false;

    VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, m_image, &mem_reqs);
	if (!CreateVulkanStagingBuffer(mem_reqs.size)) return false;

    m_imageInfo.imageView = m_imageView;
    m_imageInfo.sampler = m_sampler;
    m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	const void* imageData = image->GetDataPointer();
	uint32 imageSize = image->GetPitch() * m_height;

	return CopyFromData(imageData, imageSize);
}

bool VulkanRenderTexture::Create2D(int32 width, int32 height, EPixelFormat fmt)
{	
	m_width = width;
	m_height = height;
	m_fmt = fmt;
	m_eleSize = GetPFSize(m_fmt);

	if (!CreateVulkanImage(VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)) return false;
	if (!CreateVulkanSampler()) return false;

    VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, m_image, &mem_reqs);
	if (!CreateVulkanStagingBuffer(mem_reqs.size)) return false;

    m_imageInfo.imageView = m_imageView;
    m_imageInfo.sampler = m_sampler;
    m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    return true;
}

bool VulkanRenderTexture::CreateColorAttatchment(int32 width, int32 height, EPixelFormat fmt)
{
	m_width = width;
	m_height = height;
	m_fmt = fmt;
	m_eleSize = GetPFSize(m_fmt);

	int32 usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	if (!CreateVulkanImage(VK_IMAGE_ASPECT_COLOR_BIT, usage)) return false;
	if (!CreateVulkanSampler()) return false;

    VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, m_image, &mem_reqs);
	if (!CreateVulkanStagingBuffer(mem_reqs.size)) return false;

    m_imageInfo.imageView = m_imageView;
    m_imageInfo.sampler = m_sampler;
    m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	return true;
}

bool VulkanRenderTexture::CreateDepthAttachment(int32 width, int32 height, EPixelFormat fmt)
{
	m_width = width;
	m_height = height;
	m_fmt = fmt;
	m_eleSize = GetPFSize(m_fmt);

	if (!CreateVulkanImage(VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)) return false;
	if (!CreateVulkanSampler()) return false;

    VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, m_image, &mem_reqs);
	if (!CreateVulkanStagingBuffer(mem_reqs.size)) return false;

    m_imageInfo.imageView = m_imageView;
    m_imageInfo.sampler = m_sampler;
    m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    return true;
}

bool VulkanRenderTexture::CopyFromData(const void* pImageData, uint32 nDataSize)
{
	VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, m_image, &mem_reqs);
    VkFlags requirementsMask = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    uint8_t* data = nullptr;
    VkResult res = vkMapMemory(m_device, m_stagingMemory, 0, mem_reqs.size, 0, (void**)&data);
	if (res != VK_SUCCESS) return false;

    memcpy(data, pImageData, nDataSize);

    vkUnmapMemory(m_device, m_stagingMemory);

	m_isNeedUpdate = true;

	return true;
}

bool VulkanRenderTexture::CopyRectFromData(const void* pData, uint32 nDataSize, const Rect2Di& rt)
{
	m_isNeedUpdate = true;

	VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, m_image, &mem_reqs);
    VkFlags requirementsMask = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    uint8_t* data = nullptr;
    VkResult res = vkMapMemory(m_device, m_stagingMemory, 0, mem_reqs.size, 0, (void**)&data);
	if (res != VK_SUCCESS) return false;

	uint32 width = rt.GetWidth();
	uint32 height = rt.GetHeight();
	uint8* src = (uint8*)pData;
	for (uint32 i = 0; i < height; i++)
	{		
		uint8* target = data + ((rt.top + i) * m_width + rt.left) * m_eleSize;
		uint32 lineSize = width * m_eleSize;
    	memcpy(target, src, lineSize);
		src += lineSize;
	}

    vkUnmapMemory(m_device, m_stagingMemory);

	return true;
}

bool VulkanRenderTexture::CopyFromTexture(RenderTexture* srcTex)
{
	m_isNeedUpdate = true;

	return false;
}

bool VulkanRenderTexture::CopyRectFromTexture(RenderTexture* srcTex, const Rect2Di& srcRect, const Rect2Di& destRect)
{
	m_isNeedUpdate = true;

	return true;
}

void VulkanRenderTexture::SetSampleState(const SamplerStateDesc& state)
{

}

bool VulkanRenderTexture::CreateVulkanImage(VkImageAspectFlags aspect, VkImageUsageFlags usage)
{
	VkFormat format = PixelFormat2VkFormat(m_fmt);

	/////////////////////////////////////////////////////////////////////////////////////////
    // Create optimal tiled target image on the device
    VkImageCreateInfo image_create_info = {};
	image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.format = format;
    image_create_info.mipLevels = m_mipLevels;
    image_create_info.arrayLayers = 1;
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.extent = {m_width, m_height, 1};
    image_create_info.usage = usage;//VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    VkResult res = vkCreateImage(m_device, &image_create_info, nullptr, &m_image);
	if (res != VK_SUCCESS) return false;
    /////////////////////////////////////////////////////////////////////////////////////////////

	CreateVulkanDeviceMemory();

	/////////////////////////////////////////////////////////////////////////////////////////////
	VkImageViewCreateInfo view = {};
    view.sType          = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view.viewType       = VK_IMAGE_VIEW_TYPE_2D;
	view.format         = format;;
	view.components     = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
	view.subresourceRange.aspectMask     = aspect;//VK_IMAGE_ASPECT_COLOR_BIT;
	view.subresourceRange.baseMipLevel   = 0;
	view.subresourceRange.baseArrayLayer = 0;
	view.subresourceRange.layerCount     = 1;
	view.subresourceRange.levelCount = m_mipLevels;
	view.image = m_image;
	res = vkCreateImageView(m_device, &view, nullptr, &m_imageView);
	if (res != VK_SUCCESS) return false;
	/////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

bool VulkanRenderTexture::CreateVulkanSampler()
{
	VkSamplerCreateInfo samplerCreateInfo = {};
    samplerCreateInfo.sType               = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.maxAnisotropy       = 1.0f;
	samplerCreateInfo.magFilter           = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter           = VK_FILTER_LINEAR;
	samplerCreateInfo.mipmapMode          = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU        = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeV        = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeW        = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.mipLodBias          = 0.0f;
	samplerCreateInfo.compareOp           = VK_COMPARE_OP_NEVER;
	samplerCreateInfo.minLod              = 0.0f;
	samplerCreateInfo.maxLod              = (float) m_mipLevels;
	samplerCreateInfo.maxAnisotropy       = 16;//get_device().get_gpu().get_properties().limits.maxSamplerAnisotropy;
	samplerCreateInfo.anisotropyEnable    = VK_TRUE;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

	VkResult res = vkCreateSampler(m_device, &samplerCreateInfo, nullptr, &m_sampler);
	if (res != VK_SUCCESS) return false;

	return true;
}

bool VulkanRenderTexture::CreateVulkanStagingBuffer(uint32 imageSize)
{
	/////////////////////////////////////////////////////////////////////////////////////////
    VkBufferCreateInfo buffer_create_info = {};
	buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size = imageSize;
    buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VkResult res = vkCreateBuffer(m_device, &buffer_create_info, nullptr, &m_stagingBuffer);
	if (res != VK_SUCCESS) return false;
	/////////////////////////////////////////////////////////////////////////////////////////	

	/////////////////////////////////////////////////////////////////////////////////////////
	VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(m_device, m_stagingBuffer, &mem_reqs);
    VkFlags requirementsMask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

	VkMemoryAllocateInfo memory_allocate_info = {};
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;	
    memory_allocate_info.allocationSize = mem_reqs.size;
    memory_allocate_info.memoryTypeIndex = sInitHelper->GetMemoryTypeIndex(mem_reqs.memoryTypeBits, requirementsMask); 
    res = vkAllocateMemory(m_device, &memory_allocate_info, nullptr, &m_stagingMemory);
	if (res != VK_SUCCESS) return false;
	/////////////////////////////////////////////////////////////////////////////////////////

	res = vkBindBufferMemory(m_device, m_stagingBuffer, m_stagingMemory, 0);
	if (res != VK_SUCCESS) return false;

    return true;
}

bool VulkanRenderTexture::CreateVulkanDeviceMemory()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, m_image, &mem_reqs);
    VkFlags requirementsMask = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    
    VkMemoryAllocateInfo memory_allocate_info = {};
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;	    
	memory_allocate_info.allocationSize  = mem_reqs.size;
	memory_allocate_info.memoryTypeIndex = sInitHelper->GetMemoryTypeIndex(mem_reqs.memoryTypeBits, requirementsMask);
    VkResult res = vkAllocateMemory(m_device, &memory_allocate_info, nullptr, &m_deviceMemory);
	if (res != VK_SUCCESS) return false;
    
    res = vkBindImageMemory(m_device, m_image, m_deviceMemory, 0);
	if (res != VK_SUCCESS) return false;
	/////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

bool VulkanRenderTexture::FlushVulkanImage()
{
	if (!m_isNeedUpdate) return false;

    VkCommandBufferAllocateInfo cmd_buf_allocate_info = {};
	cmd_buf_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmd_buf_allocate_info.commandPool        = m_cmdPool;
	cmd_buf_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmd_buf_allocate_info.commandBufferCount = 1;

	VkCommandBuffer command_buffer;
	VkResult res = vkAllocateCommandBuffers(m_device, &cmd_buf_allocate_info, &command_buffer);


    VkCommandBufferBeginInfo command_buffer_info = {};
	command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    res = vkBeginCommandBuffer(command_buffer, &command_buffer_info);


    VkBufferImageCopy buffer_copy_region = {};
    buffer_copy_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    buffer_copy_region.imageSubresource.mipLevel = 0;
    buffer_copy_region.imageSubresource.baseArrayLayer = 0;
    buffer_copy_region.imageSubresource.layerCount = 1;
    buffer_copy_region.imageExtent.width = m_width;
    buffer_copy_region.imageExtent.height = m_height;
    buffer_copy_region.imageExtent.depth = 1;
    buffer_copy_region.bufferOffset = 0;

    VkImageSubresourceRange subresource_range = {};
    subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresource_range.baseMipLevel = 0;
    subresource_range.levelCount = 1;
    subresource_range.layerCount = 1;

    VkImageMemoryBarrier image_memory_barrier = {};
	image_memory_barrier.sType                  = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	image_memory_barrier.srcQueueFamilyIndex    = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.dstQueueFamilyIndex    = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.image                  = m_image;
	image_memory_barrier.subresourceRange       = subresource_range;
	image_memory_barrier.srcAccessMask          = 0;
	image_memory_barrier.dstAccessMask          = VK_ACCESS_TRANSFER_WRITE_BIT;
	image_memory_barrier.oldLayout              = VK_IMAGE_LAYOUT_UNDEFINED;
	image_memory_barrier.newLayout              = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    vkCmdPipelineBarrier(command_buffer, 
        VK_PIPELINE_STAGE_HOST_BIT, 
        VK_PIPELINE_STAGE_TRANSFER_BIT, 
        0, 
        0, nullptr, 
        0, nullptr, 
        1, &image_memory_barrier);

    vkCmdCopyBufferToImage(
        command_buffer,
        m_stagingBuffer,
        m_image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
        &buffer_copy_region);

	image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	image_memory_barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	image_memory_barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	vkCmdPipelineBarrier(
        command_buffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &image_memory_barrier);


	res = vkEndCommandBuffer(command_buffer);


	VkFenceCreateInfo fence_info{};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = 0;

	VkFence fence;
	res = vkCreateFence(m_device, &fence_info, nullptr, &fence);

	VkSubmitInfo submit_info = {};
	submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers    = &command_buffer;
	VkResult result = vkQueueSubmit(m_queue, 1, &submit_info, fence);

	res = vkWaitForFences(m_device, 1, &fence, VK_TRUE, 100000000000);

	vkDestroyFence(m_device, fence, nullptr);
    vkFreeCommandBuffers(m_device, m_cmdPool, 1, &command_buffer);

	m_isNeedUpdate = false;

    return true;
}

bool VulkanRenderTexture::Save(const char8* path)
{
    VkCommandBufferAllocateInfo cmd_buf_allocate_info = {};
	cmd_buf_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmd_buf_allocate_info.commandPool        = m_cmdPool;
	cmd_buf_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmd_buf_allocate_info.commandBufferCount = 1;

	VkCommandBuffer command_buffer;
	VkResult res = vkAllocateCommandBuffers(m_device, &cmd_buf_allocate_info, &command_buffer);


    VkCommandBufferBeginInfo command_buffer_info = {};
	command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    res = vkBeginCommandBuffer(command_buffer, &command_buffer_info);


    VkBufferImageCopy buffer_copy_region = {};
    buffer_copy_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    buffer_copy_region.imageSubresource.mipLevel = 0;
    buffer_copy_region.imageSubresource.baseArrayLayer = 0;
    buffer_copy_region.imageSubresource.layerCount = 1;
    buffer_copy_region.imageExtent.width = m_width;
    buffer_copy_region.imageExtent.height = m_height;
    buffer_copy_region.imageExtent.depth = 1;
    buffer_copy_region.bufferOffset = 0;

    VkImageSubresourceRange subresource_range = {};
    subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresource_range.baseMipLevel = 0;
    subresource_range.levelCount = 1;
    subresource_range.layerCount = 1;

    VkImageMemoryBarrier image_memory_barrier = {};
	image_memory_barrier.sType                  = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	image_memory_barrier.srcQueueFamilyIndex    = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.dstQueueFamilyIndex    = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.image                  = m_image;
	image_memory_barrier.subresourceRange       = subresource_range;
	image_memory_barrier.srcAccessMask          = 0;
	image_memory_barrier.dstAccessMask          = VK_ACCESS_TRANSFER_WRITE_BIT;
	image_memory_barrier.oldLayout              = VK_IMAGE_LAYOUT_UNDEFINED;
	image_memory_barrier.newLayout              = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

    vkCmdPipelineBarrier(command_buffer, 
        VK_PIPELINE_STAGE_HOST_BIT, 
        VK_PIPELINE_STAGE_TRANSFER_BIT, 
        0, 
        0, nullptr, 
        0, nullptr, 
        1, &image_memory_barrier);

    vkCmdCopyImageToBuffer(
        command_buffer,
		m_image, 
		VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 
        m_stagingBuffer,
		1,
        &buffer_copy_region);


	res = vkEndCommandBuffer(command_buffer);

	VkFenceCreateInfo fence_info{};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = 0;

	VkFence fence;
	res = vkCreateFence(m_device, &fence_info, nullptr, &fence);

	VkSubmitInfo submit_info = {};
	submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers    = &command_buffer;
	VkResult result = vkQueueSubmit(m_queue, 1, &submit_info, fence);

	res = vkWaitForFences(m_device, 1, &fence, VK_TRUE, 100000000000);

	vkDestroyFence(m_device, fence, nullptr);
    vkFreeCommandBuffers(m_device, m_cmdPool, 1, &command_buffer);


	VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, m_image, &mem_reqs);
    VkFlags requirementsMask = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    uint8_t* data = nullptr;
    res = vkMapMemory(m_device, m_stagingMemory, 0, mem_reqs.size, 0, (void**)&data);
	if (res != VK_SUCCESS) return false;

	Image* image = (Image*)sResMgr->GenerateResource(ResType::ERT_IMAGE);
	image->CreateImage(m_width, m_height);
	uint8* buffer = (uint8*)image->GetDataPointer();	

	memcpy(buffer, data, m_width * m_height * 4);
	
	image->Save(path);

    vkUnmapMemory(m_device, m_stagingMemory);

    return true;
}

