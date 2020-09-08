#include "PreCompile.h"
#include "Pharos.h"

VulkanRenderTexture::VulkanRenderTexture(VkDevice device, VkCommandPool cmdPool, VkQueue queue)
{
	m_device = device;
    m_cmdPool = cmdPool;
    m_queue = queue;

	m_isNeedUpdate = false;

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
    m_width = image->GetWidth();
    m_height = image->GetHeight();
	m_fmt = EPF_BGRA8_UNORM;
    m_eleSize = GetPFSize(m_fmt);

	const void* imageData = image->GetDataPointer();
	uint32 imageSize = image->GetPitch() * m_height;

	m_mipLevels = 1;
	m_copyRegions.clear();

	//注意!! 目前只支持 mip level 等于 1 , 其他值会出错
    for (uint32_t i = 0; i < m_mipLevels; i++)
    {
        uint64 offset = 0; //ktxTexture_GetImageOffset(ktx_texture, i, 0, 0, &offset);
        VkBufferImageCopy buffer_copy_region = {};
        buffer_copy_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        buffer_copy_region.imageSubresource.mipLevel = i;
        buffer_copy_region.imageSubresource.baseArrayLayer = 0;
        buffer_copy_region.imageSubresource.layerCount = 1;
        buffer_copy_region.imageExtent.width = m_width >> i;
        buffer_copy_region.imageExtent.height = m_height >> i;
        buffer_copy_region.imageExtent.depth = 1;
        buffer_copy_region.bufferOffset = offset;
        m_copyRegions.push_back(buffer_copy_region);
    }

	if (!CreateVulkanImage())
	{
		return false;
	}

	if (!CreateVulkanSampler())
	{
		return false;
	}

    VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, m_image, &mem_reqs);

	if (!CreateVulkanStagingBuffer(mem_reqs.size))
	{
		return false;
	}

    m_imageInfo.imageView = m_imageView;
    m_imageInfo.sampler = m_sampler;
    m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	return CopyFromData(imageData, imageSize);
}

bool VulkanRenderTexture::Create(int32 width, int32 height, EPixelFormat fmt)
{	
	m_width = width;
	m_height = height;
	m_fmt = fmt;
	m_eleSize = GetPFSize(m_fmt);

	m_mipLevels = 1;
	m_copyRegions.clear();

	//注意!! 目前只支持 mip level 等于 1 , 其他值会出错
    for (uint32_t i = 0; i < m_mipLevels; i++)
    {
        uint64 offset = 0; //ktxTexture_GetImageOffset(ktx_texture, i, 0, 0, &offset);
        VkBufferImageCopy buffer_copy_region = {};
        buffer_copy_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        buffer_copy_region.imageSubresource.mipLevel = i;
        buffer_copy_region.imageSubresource.baseArrayLayer = 0;
        buffer_copy_region.imageSubresource.layerCount = 1;
        buffer_copy_region.imageExtent.width = m_width >> i;
        buffer_copy_region.imageExtent.height = m_height >> i;
        buffer_copy_region.imageExtent.depth = 1;
        buffer_copy_region.bufferOffset = offset;
        m_copyRegions.push_back(buffer_copy_region);
    }

	if (!CreateVulkanImage())
	{
		return false;
	}

	if (!CreateVulkanSampler())
	{
		return false;
	}

    VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, m_image, &mem_reqs);

	if (!CreateVulkanStagingBuffer(mem_reqs.size))
	{
		return false;
	}

    m_imageInfo.imageView = m_imageView;
    m_imageInfo.sampler = m_sampler;
    m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

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

void VulkanRenderTexture::SetSampleState(RenderSamplerState* state)
{
}

bool VulkanRenderTexture::CreateVulkanStagingBuffer(uint32 imageSize)
{
	/////////////////////////////////////////////////////////////////////////////////////////
    VkBufferCreateInfo buffer_create_info = {};
	buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size = imageSize;
    buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
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
	samplerCreateInfo.maxLod              = (float) m_copyRegions.size();
	samplerCreateInfo.maxAnisotropy       = 16;//get_device().get_gpu().get_properties().limits.maxSamplerAnisotropy;
	samplerCreateInfo.anisotropyEnable    = VK_TRUE;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

	VkResult res = vkCreateSampler(m_device, &samplerCreateInfo, nullptr, &m_sampler);
	if (res != VK_SUCCESS) return false;

	return true;
}

bool VulkanRenderTexture::CreateVulkanImage()
{
	VkFormat format = PixelFormat2MetalFormat(m_fmt);

	/////////////////////////////////////////////////////////////////////////////////////////
    // Create optimal tiled target image on the device
    VkImageCreateInfo image_create_info = {};
	image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.format = format;//VK_FORMAT_B8G8R8A8_UNORM;
    image_create_info.mipLevels = m_copyRegions.size();
    image_create_info.arrayLayers = 1;
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.extent = {m_width, m_height, 1};
    image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    VkResult res = vkCreateImage(m_device, &image_create_info, nullptr, &m_image);
	if (res != VK_SUCCESS) return false;
    /////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////
	VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(m_device, m_image, &mem_reqs);
    VkFlags requirementsMask = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    
    VkMemoryAllocateInfo memory_allocate_info = {};
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;	    
	memory_allocate_info.allocationSize  = mem_reqs.size;
	memory_allocate_info.memoryTypeIndex = sInitHelper->GetMemoryTypeIndex(mem_reqs.memoryTypeBits, requirementsMask);
    res = vkAllocateMemory(m_device, &memory_allocate_info, nullptr, &m_deviceMemory);
	if (res != VK_SUCCESS) return false;
    
    res = vkBindImageMemory(m_device, m_image, m_deviceMemory, 0);
	if (res != VK_SUCCESS) return false;
	/////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////
	VkImageViewCreateInfo view = {};
    view.sType          = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view.viewType       = VK_IMAGE_VIEW_TYPE_2D;
	view.format         = format;//VK_FORMAT_B8G8R8A8_UNORM;
	view.components     = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
	view.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
	view.subresourceRange.baseMipLevel   = 0;
	view.subresourceRange.baseArrayLayer = 0;
	view.subresourceRange.layerCount     = 1;
	view.subresourceRange.levelCount = m_copyRegions.size();
	view.image = m_image;
	res = vkCreateImageView(m_device, &view, nullptr, &m_imageView);
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


	VkImageSubresourceRange subresource_range = {};
	subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresource_range.baseMipLevel = 0;
	subresource_range.levelCount = m_copyRegions.size();
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
        static_cast<uint32_t>(m_copyRegions.size()),
        m_copyRegions.data());

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