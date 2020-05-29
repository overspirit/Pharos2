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

			VkFormat			m_surfaceFormat;
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
			VkInstance CreateInstance();
			VkPhysicalDevice EnumPhysicalDevice(VkInstance inst);
			uint32 GetGraphicsQueueFamilyIndex(VkPhysicalDevice gpu, VkSurfaceKHR surface);
			VkFormat GetPhysicalDeviceSurfaceFormat(VkPhysicalDevice gpu, VkSurfaceKHR surface);
			VkDevice CreateDevice(VkPhysicalDevice gpu, uint32 queueFamilyIndex);
			VkSwapchainKHR CreateSwapchain(VkPhysicalDevice gpu, VkDevice device, VkSurfaceKHR surface, VkFormat surfaceFormat);

		private:
			static VkBool32 VulkanDebugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
				VkDebugUtilsMessageTypeFlagsEXT message_type, 
				const VkDebugUtilsMessengerCallbackDataEXT* callback_data, 
				void* user_data);

		public:
			bool Initialize(glfw_window* window);
			void Destroy();

			VkDevice GetVulkanDevice() { return m_device; }
			VkQueue GetVulkanQueue() { return m_queue; }

			uint32 GetQueueFamilyIndex() { return m_queueFamilyIndex; }
			VkSwapchainKHR GetSwapchain() { return m_swapchain; }
			int32 GetWindowSurfaceWidth() { return m_surfaceWidth; }
			int32 GetWindowSurfaceHeight() { return m_surfaceHeight; }
			VkFormat GetWindowSurfaceFormat() { return m_surfaceFormat; }

			uint32 GetMemoryTypeIndex(uint32 typeBits, VkFlags requirementsMask);
		};
	}
}
