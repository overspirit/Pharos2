#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderTexture::VulkanRenderTexture(VkDevice device, VkCommandPool cmdPool, VkQueue queue)
{
	m_device = device;
    m_cmdPool = cmdPool;
    m_queue = queue;

    m_mipLevels = 1;
}

VulkanRenderTexture::~VulkanRenderTexture()
{
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
	uint32 width = image->GetWidth();
	uint32 height = image->GetHeight();
	const void* imageData = image->GetDataPointer();
	uint32 imageSize = image->GetPitch() * height;

    VkBuffer staging_buffer = CreateStagingBuffer(imageData, imageSize);


    std::vector<VkBufferImageCopy> buffer_copy_regions;

    for (uint32_t i = 0; i < m_mipLevels; i++)
    {
        uint64 offset = 0;
        //KTX_error_code result = ktxTexture_GetImageOffset(ktx_texture, i, 0, 0, &offset);
        VkBufferImageCopy buffer_copy_region = {};
        buffer_copy_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        buffer_copy_region.imageSubresource.mipLevel = i;
        buffer_copy_region.imageSubresource.baseArrayLayer = 0;
        buffer_copy_region.imageSubresource.layerCount = 1;
        buffer_copy_region.imageExtent.width = width >> i;
        buffer_copy_region.imageExtent.height = height >> i;
        buffer_copy_region.imageExtent.depth = 1;
        buffer_copy_region.bufferOffset = offset;
        buffer_copy_regions.push_back(buffer_copy_region);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Create optimal tiled target image on the device
    VkImageCreateInfo image_create_info = {};
	image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.format = VK_FORMAT_B8G8R8A8_UNORM;
    image_create_info.mipLevels = m_mipLevels;
    image_create_info.arrayLayers = 1;
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    // Set initial layout of the image to undefined
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.extent = {width, height, 1};
    image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    VkResult res = vkCreateImage(m_device, &image_create_info, nullptr, &m_image);
    /////////////////////////////////////////////////////////////////////////////////////////////

    VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, m_image, &mem_reqs);
    VkFlags requirementsMask = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    
    VkMemoryAllocateInfo memory_allocate_info = {};
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;	
    
	memory_allocate_info.allocationSize  = mem_reqs.size;
	memory_allocate_info.memoryTypeIndex = sInitHelper->GetMemoryTypeIndex(mem_reqs.memoryTypeBits, requirementsMask);
    vkAllocateMemory(m_device, &memory_allocate_info, nullptr, &m_deviceMemory);
    
    vkBindImageMemory(m_device, m_image, m_deviceMemory, 0);

    CopyStagingImage(staging_buffer, buffer_copy_regions);
	
    // Clean up staging resources
	//vkFreeMemory(m_device, staging_memory, nullptr);
	vkDestroyBuffer(m_device, staging_buffer, nullptr);

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

	VkSampler sampler;
	res = vkCreateSampler(m_device, &samplerCreateInfo, nullptr, &sampler);
    



    VkImageViewCreateInfo view = {};
    view.sType          = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view.viewType       = VK_IMAGE_VIEW_TYPE_2D;
	view.format         = VK_FORMAT_B8G8R8A8_UNORM;
	view.components     = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
	view.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
	view.subresourceRange.baseMipLevel   = 0;
	view.subresourceRange.baseArrayLayer = 0;
	view.subresourceRange.layerCount     = 1;
	view.subresourceRange.levelCount = m_mipLevels;
	view.image = m_image;

	VkImageView imageView;
	vkCreateImageView(m_device, &view, nullptr, &imageView);

    m_width = width;
    m_height = height;
    m_eleSize = 32;

	// Store current layout for later reuse
    VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    m_imageInfo.imageView = imageView;
    m_imageInfo.sampler = sampler;
    m_imageInfo.imageLayout = imageLayout;

    return true;
}


VkBuffer VulkanRenderTexture::CreateStagingBuffer(const void* imageData, uint32 imageSize)
{
	/////////////////////////////////////////////////////////////////////////////////////////
    VkBuffer staging_buffer;

    VkBufferCreateInfo buffer_create_info = {};
	buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size = imageSize;
    buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkCreateBuffer(m_device, &buffer_create_info, nullptr, &staging_buffer);
	/////////////////////////////////////////////////////////////////////////////////////////

	

	/////////////////////////////////////////////////////////////////////////////////////////
	VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(m_device, staging_buffer, &mem_reqs);
    VkFlags requirementsMask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    VkDeviceMemory staging_memory;

	VkMemoryAllocateInfo memory_allocate_info = {};
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;	
    memory_allocate_info.allocationSize = mem_reqs.size;
    memory_allocate_info.memoryTypeIndex = sInitHelper->GetMemoryTypeIndex(mem_reqs.memoryTypeBits, requirementsMask); 
    vkAllocateMemory(m_device, &memory_allocate_info, nullptr, &staging_memory);
	/////////////////////////////////////////////////////////////////////////////////////////

	vkBindBufferMemory(m_device, staging_buffer, staging_memory, 0);

    uint8_t *data;
    vkMapMemory(m_device, staging_memory, 0, mem_reqs.size, 0, (void**)&data);
    memcpy(data, imageData, imageSize);
    vkUnmapMemory(m_device, staging_memory);

    return staging_buffer;
}

bool VulkanRenderTexture::CopyStagingImage(VkBuffer staging_buffer, const vector<VkBufferImageCopy>& buffer_copy_regions)
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




	// Image memory barriers for the texture image

	// The sub resource range describes the regions of the image that will be transitioned using the memory barriers below
	VkImageSubresourceRange subresource_range = {};
	subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresource_range.baseMipLevel = 0;
	subresource_range.levelCount = m_mipLevels;
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

    vkCmdPipelineBarrier(
        command_buffer, 
        VK_PIPELINE_STAGE_HOST_BIT, 
        VK_PIPELINE_STAGE_TRANSFER_BIT, 
        0, 
        0, nullptr, 
        0, nullptr, 
        1, &image_memory_barrier);

    vkCmdCopyBufferToImage(
        command_buffer,
        staging_buffer,
        m_image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        static_cast<uint32_t>(buffer_copy_regions.size()),
        buffer_copy_regions.data());

    // Once the data has been uploaded we transfer to the texture image to the shader read layout, so it can be sampled from
	image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	image_memory_barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	image_memory_barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	// Insert a memory dependency at the proper pipeline stages that will execute the image layout transition
	// Source pipeline stage stage is copy command exection (VK_PIPELINE_STAGE_TRANSFER_BIT)
	// Destination pipeline stage fragment shader access (VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT)
	vkCmdPipelineBarrier(
        command_buffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &image_memory_barrier);


	res = vkEndCommandBuffer(command_buffer);

	VkSubmitInfo submit_info = {};
	submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers    = &command_buffer;

	// Create fence to ensure that the command buffer has finished executing
	VkFenceCreateInfo fence_info{};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = 0;

	VkFence fence;
	res = vkCreateFence(m_device, &fence_info, nullptr, &fence);

	// Submit to the queue
	VkResult result = vkQueueSubmit(m_queue, 1, &submit_info, fence);

	// Wait for the fence to signal that command buffer has finished executing
	res = vkWaitForFences(m_device, 1, &fence, VK_TRUE, 100000000000);

	vkDestroyFence(m_device, fence, nullptr);
    vkFreeCommandBuffers(m_device, m_cmdPool, 1, &command_buffer);

    return true;
}

bool VulkanRenderTexture::Create(int32 width, int32 height, EPixelFormat fmt)
{
	


    return true;
}

//bool VulkanRenderTexture::CreateDepthTexture(int32 width, int32 height)
//{
//
//    return true;
//}
//
//bool VulkanRenderTexture::CreateTargetTexture(int32 width, int32 height, EPixelFormat fmt)
//{
//    return true;
//}


bool VulkanRenderTexture::CopyFromData(const void* pImageData, uint32 nDataSize)
{

	return true;
}

bool VulkanRenderTexture::CopyRectFromData(const void* pData, uint32 nDataSize, const Rect2Di& rt)
{
	return true;
}

bool VulkanRenderTexture::CopyFromTexture(RenderTexture* srcTex)
{
	return false;
}

bool VulkanRenderTexture::CopyRectFromTexture(RenderTexture* srcTex, const Rect2Di& srcRect, const Rect2Di& destRect)
{
	return true;
}

void VulkanRenderTexture::SetSampleState(RenderSamplerState* state)
{
}