#pragma once

namespace Pharos
{
	namespace Render
	{
		class VulkanShaderProgram : public RenderProgram
		{
		public:
			VulkanShaderProgram(VkDevice device);
			virtual ~VulkanShaderProgram();

		private:
			VkDevice	m_device;

			map<string, vector<uint32>>		m_shaderDatas;

			VkShaderModule		m_vertShader;
			VkShaderModule		m_fragShader;

			VkPipelineShaderStageCreateInfo		m_shaderStages[2];

		public:
			VkPipelineShaderStageCreateInfo* GetShaderStage() { return m_shaderStages; }

		public:
			virtual bool SetLibraryWithPath(const char8* libPath);
			virtual bool SetSourceWithPath(const char8* sourcePath);
			virtual bool SetSource(const char8* source);
			virtual bool CompileVertexFunctionWithName(const char8* name);
			virtual bool CompileFragmentFunctionWithName(const char8* name);
		};
	}
}
