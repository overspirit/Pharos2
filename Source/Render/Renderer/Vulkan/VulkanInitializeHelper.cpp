#include "PreCompile.h"
#include "Pharos.h"

VulkanInitializeHelper::VulkanInitializeHelper(void)
{
	m_inst = VK_NULL_HANDLE;
	m_gpu = VK_NULL_HANDLE;
	m_device = VK_NULL_HANDLE;
	
	m_gpuMemSize = 0;

	m_swapchain = VK_NULL_HANDLE;

	m_colorFormat = VK_FORMAT_MAX_ENUM;
	m_depthFormat = VK_FORMAT_MAX_ENUM;

	m_surface = VK_NULL_HANDLE;
	m_surfaceWidth = 0;
	m_surfaceHeight = 0;

	m_queue = VK_NULL_HANDLE;
	m_queueFamilyIndex = -1;
	
	m_debugMsger = VK_NULL_HANDLE;
	m_callback = VK_NULL_HANDLE;
}

VulkanInitializeHelper::~VulkanInitializeHelper(void)
{
}

bool VulkanInitializeHelper::Initialize(MyWindow* window)
{
	const char* sourface_extension = window->get_surface_extension();

	m_inst = CreateInstance(sourface_extension);
	if (m_inst == VK_NULL_HANDLE) return false;

	m_gpu = EnumPhysicalDevice(m_inst);
	if(m_gpu == VK_NULL_HANDLE) return false;

	m_surface = window->create_surface(m_inst);
	if(m_surface == VK_NULL_HANDLE) return false;

	m_queueFamilyIndex = GetGraphicsQueueFamilyIndex(m_gpu, m_surface);
	if(m_queueFamilyIndex == 0xFFFFFFFF) return false;

	m_colorFormat = GetPhysicalDeviceSurfaceColorFormat(m_gpu, m_surface);
	if(m_colorFormat == VK_FORMAT_MAX_ENUM) return false;
	
	m_depthFormat = GetPhysicalDeviceSurfaceDepthFormat(m_gpu);
	if(m_depthFormat == VK_FORMAT_MAX_ENUM) return false;

	m_device = CreateDevice(m_gpu, m_queueFamilyIndex);
	if(m_device == VK_NULL_HANDLE) return false;

	vkGetDeviceQueue(m_device, m_queueFamilyIndex, 0, &m_queue);
	if(m_queue == VK_NULL_HANDLE) return false;

	m_swapchain = CreateSwapchain(m_gpu, m_device, m_surface, m_colorFormat);
	if(m_swapchain == VK_NULL_HANDLE) return false;

	return true;
}

VkBool32 VulkanInitializeHelper::VulkanDebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
	VkDebugUtilsMessageTypeFlagsEXT message_type, 
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data, 
	void* user_data)
{
	const char8* msg = callback_data->pMessage;
	File* logFile = (File*)user_data;
	logFile->Write(msg, strlen(msg));
	logFile->Write("\n", 1);
	logFile->Flush();

	printf("%s\n", msg);
	
	return VK_FALSE;
}

VkBool32 vkDebugReportCallbackEXT(VkDebugReportFlagsEXT flags, 
	VkDebugReportObjectTypeEXT objectType,
	uint64_t object,
	size_t location,
	int32_t messageCode,
	const char* pLayerPrefix,
	const char* pMessage,
	void* pUserData)
{
	printf("%s\n", pMessage);
	
	return VK_FALSE;
}

VkInstance VulkanInitializeHelper::CreateInstance(const char* sourface_extension)
{
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = NULL;
	app_info.pApplicationName = "Pharos Application";
	app_info.applicationVersion = 1;
	app_info.pEngineName = "Pharos Engine";
	app_info.engineVersion = 1;
	app_info.apiVersion = VK_API_VERSION_1_0;
	
	//注意这里，Vulkan 是 基于各种扩展的。。。
	std::vector<const char*> exts;
	exts.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	exts.push_back(sourface_extension);

	std::vector<const char*> validationLayers;

	uint32_t instance_layer_count;
	vkEnumerateInstanceLayerProperties(&instance_layer_count, nullptr);

	std::vector<VkLayerProperties> supported_validation_layers(instance_layer_count);
	vkEnumerateInstanceLayerProperties(&instance_layer_count, supported_validation_layers.data());

#if defined(_LINUX_PLATFORM_)
	exts.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	//exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	validationLayers.push_back("VK_LAYER_KHRONOS_validation");
	//validationLayers.push_back("VK_LAYER_LUNARG_vktrace");
	//validationLayers.push_back("VK_LAYER_LUNARG_device_simulation");
	// validationLayers.push_back("VK_LAYER_LUNARG_monitor");
	//validationLayers.push_back("VK_LAYER_LUNARG_api_dump");
	// validationLayers.push_back("VK_LAYER_LUNARG_screenshot");

#endif

	VkInstanceCreateInfo inst_info = {};
	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pNext = NULL;
	inst_info.flags = 0;
	inst_info.pApplicationInfo = &app_info;
	inst_info.enabledExtensionCount = exts.size();
	inst_info.ppEnabledExtensionNames = exts.data();
	inst_info.enabledLayerCount = validationLayers.size();
	inst_info.ppEnabledLayerNames = validationLayers.data();

	VkInstance inst;
	VkResult res = vkCreateInstance(&inst_info, NULL, &inst);
	if (res != VK_SUCCESS) return VK_NULL_HANDLE;

#if defined(_LINUX_PLATFORM_)
	m_logFile.Create("log.txt", false);

	VkDebugUtilsMessengerCreateInfoEXT debugUtilsInfo = {};
    debugUtilsInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugUtilsInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugUtilsInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugUtilsInfo.pfnUserCallback = (PFN_vkDebugUtilsMessengerCallbackEXT)VulkanDebugCallback;
	debugUtilsInfo.pUserData = &m_logFile;
	// auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(inst, "vkCreateDebugUtilsMessengerEXT");
	// res = func(inst, &debugUtilsInfo, nullptr, &m_debugMsger);
	// if (res != VK_SUCCESS)
	// {
	// 	return VK_NULL_HANDLE;
	// }

	//VkDebugReportCallbackEXT

	VkDebugReportCallbackCreateInfoEXT createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.pNext = nullptr;
    createInfo.flags = 
		VK_DEBUG_REPORT_INFORMATION_BIT_EXT | 
		VK_DEBUG_REPORT_WARNING_BIT_EXT | 
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT | 
		VK_DEBUG_REPORT_DEBUG_BIT_EXT;
    createInfo.pfnCallback = vkDebugReportCallbackEXT;
    createInfo.pUserData = nullptr;

	
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(inst, "vkCreateDebugReportCallbackEXT");
	res = func(inst, &createInfo, nullptr, &m_callback);
	if (res != VK_SUCCESS)
	{
		return VK_NULL_HANDLE;
	}

#endif


	return inst;
}

void VulkanInitializeHelper::Destroy()
{
#if defined(_LINUX_PLATFORM_)

	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_inst, "vkDestroyDebugReportCallbackEXT");
	func(m_inst, m_callback, nullptr);	
	
#endif

	vkDestroySwapchainKHR(m_device, m_swapchain, NULL);

	vkDestroyDevice(m_device, NULL);
	
	vkDestroySurfaceKHR(m_inst, m_surface, NULL);

	vkDestroyInstance(m_inst, NULL);
}

void VulkanInitializeHelper::RecreateSwapchain()
{
	vkDestroySwapchainKHR(m_device, m_swapchain, NULL);
}

VkPhysicalDevice VulkanInitializeHelper::EnumPhysicalDevice(VkInstance inst)
{
	uint32_t gpu_count = 0;
	VkResult res = vkEnumeratePhysicalDevices(inst, &gpu_count, NULL);
	if (res != VK_SUCCESS) return VK_NULL_HANDLE;

	std::vector<VkPhysicalDevice> gpuList;
	gpuList.resize(gpu_count);
	res = vkEnumeratePhysicalDevices(inst, &gpu_count, gpuList.data());
	if (res != VK_SUCCESS || gpu_count < 1) return VK_NULL_HANDLE;

	VkPhysicalDevice gpu = gpuList[0];

	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(gpu, &properties);

	m_gpuName = properties.deviceName;

	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(gpu, &memoryProperties);

	for (uint32 i = 0; i < memoryProperties.memoryHeapCount; i++)
	{
		if (memoryProperties.memoryHeaps[i].flags == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		{
			m_gpuMemSize = (uint32)(memoryProperties.memoryHeaps[i].size / 1024 / 1024);
			break;
		}
	}
	
	return gpu;
}

uint32 VulkanInitializeHelper::GetGraphicsQueueFamilyIndex(VkPhysicalDevice gpu, VkSurfaceKHR surface)
{
	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, NULL);

	std::vector<VkQueueFamilyProperties> queueFamilyList;
	queueFamilyList.resize(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, queueFamilyList.data());

	for (uint32 i = 0; i < queueFamilyList.size(); i++)
	{
		if (queueFamilyList[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			VkBool32 support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &support);

			if(support) return i;
		}
	}

	return 0xFFFFFFFF;
}

VkFormat VulkanInitializeHelper::GetPhysicalDeviceSurfaceColorFormat(VkPhysicalDevice gpu, VkSurfaceKHR surface)
{
	uint32_t formatCount = 0;
	VkResult res = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, NULL);
    if(res != VK_SUCCESS) return VK_FORMAT_MAX_ENUM;

	std::vector<VkSurfaceFormatKHR> surfFormats;
	surfFormats.resize(formatCount);	
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, surfFormats.data());
	if(res != VK_SUCCESS) return VK_FORMAT_MAX_ENUM;

	VkFormat pendingFormat[] =
	{
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_FORMAT_B8G8R8A8_UNORM,
		VK_FORMAT_R8G8B8A8_SRGB,
		VK_FORMAT_B8G8R8A8_SRGB
	};

	for(auto surformat : surfFormats)
	{
		for (auto format : pendingFormat)
		{
			if(surformat.format == format)
			{
				return surformat.format;
			}
		}
	}

	return VK_FORMAT_MAX_ENUM;
}

VkFormat VulkanInitializeHelper::GetPhysicalDeviceSurfaceDepthFormat(VkPhysicalDevice gpu)
{
	VkFormat pendingFormat[] = 
	{
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT
	};    

	for (VkFormat format : pendingFormat)
	{
		VkFormatProperties props;	
		vkGetPhysicalDeviceFormatProperties(m_gpu, format, &props);

		if ((props.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) ||
			(props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT))
		{
			return format;
		}
	}

	return VK_FORMAT_MAX_ENUM;
}

VkImageTiling VulkanInitializeHelper::GetDepthImageTiling(VkFormat depthFormat)
{
	VkFormatProperties props;
	vkGetPhysicalDeviceFormatProperties(m_gpu, depthFormat, &props);
    if (props.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
	{
		return VK_IMAGE_TILING_LINEAR;
	}  
	else if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
	{
        return VK_IMAGE_TILING_OPTIMAL;
    }

	return VK_IMAGE_TILING_MAX_ENUM;
}

VkDevice VulkanInitializeHelper::CreateDevice(VkPhysicalDevice gpu, uint32 queueFamilyIndex)
{
	float queue_priorities[1] = {0.0};

	VkDeviceQueueCreateInfo queue_info = {};	
	queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_info.pNext = NULL;
	queue_info.queueCount = 1;
	queue_info.queueFamilyIndex = queueFamilyIndex;
	queue_info.pQueuePriorities = queue_priorities;

	std::vector<const char*> exts;
	exts.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(gpu, &features);

	VkDeviceCreateInfo device_info = {};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pNext = NULL;
	device_info.queueCreateInfoCount = 1;
	device_info.pQueueCreateInfos = &queue_info;
	device_info.enabledExtensionCount = exts.size();
	device_info.ppEnabledExtensionNames = exts.data();
	device_info.enabledLayerCount = 0;
	device_info.ppEnabledLayerNames = NULL;
	device_info.pEnabledFeatures = &features;

	VkDevice device;
	VkResult res = vkCreateDevice(gpu, &device_info, NULL, &device);
	if(res != VK_SUCCESS)return VK_NULL_HANDLE;

	return device;
}

VkSwapchainKHR VulkanInitializeHelper::CreateSwapchain(VkPhysicalDevice gpu, VkDevice device, VkSurfaceKHR surface, VkFormat surfaceFormat)
{
	VkSurfaceCapabilitiesKHR surfCapabilities;
    VkResult res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &surfCapabilities);
    assert(res == VK_SUCCESS);

	VkSurfaceTransformFlagBitsKHR pre_transform;
	if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		pre_transform = surfCapabilities.currentTransform;
	}

	VkCompositeAlphaFlagBitsKHR composite = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	if (surfCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
	{
		composite = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	}
	else if (surfCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR)
	{
		composite = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
	}
	else if (surfCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
	{
		composite = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
	}
	else if (surfCapabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
	{
		composite = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
	}

	VkExtent2D swapchainExtent = surfCapabilities.currentExtent;
	m_surfaceWidth = swapchainExtent.width;
	m_surfaceHeight = swapchainExtent.height;

	VkSwapchainCreateInfoKHR swapchain_ci = {};
    swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_ci.pNext = NULL;
    swapchain_ci.surface = surface;
    swapchain_ci.minImageCount = surfCapabilities.minImageCount;
    swapchain_ci.imageFormat = surfaceFormat;
    swapchain_ci.imageExtent.width = swapchainExtent.width;
    swapchain_ci.imageExtent.height = swapchainExtent.height;
    swapchain_ci.preTransform = pre_transform;
    swapchain_ci.compositeAlpha = composite;
    swapchain_ci.imageArrayLayers = 1;
    swapchain_ci.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;//VK_PRESENT_MODE_FIFO_KHR;  //垂直同步
    swapchain_ci.oldSwapchain = VK_NULL_HANDLE;
    swapchain_ci.clipped = true;
    swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchain_ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_ci.queueFamilyIndexCount = 0;
    swapchain_ci.pQueueFamilyIndices = NULL;

	VkSwapchainKHR	swapchain;
	res = vkCreateSwapchainKHR(device, &swapchain_ci, NULL, &swapchain);
    if(res != VK_SUCCESS) return VK_NULL_HANDLE;

	return swapchain;
}

uint32 VulkanInitializeHelper::GetMemoryTypeIndex(uint32 typeBits, VkFlags requirementsMask)
{
	VkPhysicalDeviceMemoryProperties memory_properties;
	vkGetPhysicalDeviceMemoryProperties(m_gpu, &memory_properties);
	
	uint32 typeIndex = 0xFFFFFFFF;

    // Search memtypes to find first index with those properties
    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)    
    {
        if ((typeBits & 1) == 1) 
        {
            // Type is available, does it match user properties?
            if ((memory_properties.memoryTypes[i].propertyFlags & requirementsMask) == requirementsMask) 
            {
                return i;
            }
        }
        typeBits >>= 1;
    }

	return 0xFFFFFFFF;
}

