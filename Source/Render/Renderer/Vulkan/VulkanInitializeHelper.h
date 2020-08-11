#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanInitializeHelper : public Utils::Singleton<VulkanInitializeHelper>
		{
		public:
			VulkanInitializeHelper(void);
			virtual ~VulkanInitializeHelper(void);

		private:
			VkInstance			m_inst;
			VkPhysicalDevice	m_gpu;

			string				m_gpuName;
			uint32				m_gpuMemSize;

			VkFormat			m_colorFormat;
			VkFormat			m_depthFormat;

			VkSurfaceKHR		m_surface;
			int32				m_surfaceWidth;
			int32				m_surfaceHeight;

			VkDevice			m_device;

			VkSwapchainKHR		m_swapchain;

			uint32				m_queueFamilyIndex;
			VkQueue				m_queue;

			File						m_logFile;
			VkDebugUtilsMessengerEXT	m_debugMsger;

		private:
			VkInstance CreateInstance(const char* sourface_extension);
			VkPhysicalDevice EnumPhysicalDevice(VkInstance inst);
			uint32 GetGraphicsQueueFamilyIndex(VkPhysicalDevice gpu, VkSurfaceKHR surface);
			VkFormat GetPhysicalDeviceSurfaceColorFormat(VkPhysicalDevice gpu, VkSurfaceKHR surface);
			VkFormat GetPhysicalDeviceSurfaceDepthFormat(VkPhysicalDevice gpu);
			VkDevice CreateDevice(VkPhysicalDevice gpu, uint32 queueFamilyIndex);
			VkSwapchainKHR CreateSwapchain(VkPhysicalDevice gpu, VkDevice device, VkSurfaceKHR surface, VkFormat surfaceFormat);

		private:
			static VkBool32 VulkanDebugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
				VkDebugUtilsMessageTypeFlagsEXT message_type, 
				const VkDebugUtilsMessengerCallbackDataEXT* callback_data, 
				void* user_data);

		public:
			bool Initialize(MyWindow* window);
			void Destroy();

			const char8* GetDefaultPhysicalDeviceName() { return m_gpuName.c_str(); }
			uint32 GetDefaultPhysicalDeviceMemorySize() { return m_gpuMemSize; }

			VkDevice GetVulkanDevice() { return m_device; }
			VkQueue GetVulkanQueue() { return m_queue; }

			uint32 GetQueueFamilyIndex() { return m_queueFamilyIndex; }
			VkSwapchainKHR GetSwapchain() { return m_swapchain; }
			int32 GetWindowSurfaceWidth() { return m_surfaceWidth; }
			int32 GetWindowSurfaceHeight() { return m_surfaceHeight; }
			VkFormat GetWindowSurfaceColorFormat() { return m_colorFormat; }
			VkFormat GetWindowSurfaceDepthFormat() { return m_depthFormat; }
			VkImageTiling GetDepthImageTiling(VkFormat depthFormat);

			uint32 GetMemoryTypeIndex(uint32 typeBits, VkFlags requirementsMask);
		};
	}
}
