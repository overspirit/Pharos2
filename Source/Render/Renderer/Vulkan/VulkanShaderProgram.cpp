#include "PreCompile.h"
#include "Pharos.h"

VulkanShaderProgram::VulkanShaderProgram(VkDevice device)
{
	m_device = device;

	m_shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	m_shaderStages[0].pNext = NULL;
	m_shaderStages[0].pSpecializationInfo = NULL;
	m_shaderStages[0].flags = 0;
	m_shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	m_shaderStages[0].pName = "main";
	m_shaderStages[0].module = VK_NULL_HANDLE;

	m_shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	m_shaderStages[1].pNext = NULL;
	m_shaderStages[1].pSpecializationInfo = NULL;
	m_shaderStages[1].flags = 0;
	m_shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	m_shaderStages[1].pName = "main";
	m_shaderStages[1].module = VK_NULL_HANDLE;
}

VulkanShaderProgram::~VulkanShaderProgram()
{
}

bool VulkanShaderProgram::SetLibraryWithPath(const char8 *libPath)
{
	string vertPath = string(libPath) + "/shaders.vert.spv";
	string fragPath = string(libPath) + "/shaders.frag.spv";

	{
		File vertFile;
		if (!vertFile.Open(vertPath.c_str()))
			return false;

		uint32 vertFileSize = vertFile.GetSize();
		vector<uint32> &vertSpvData = m_shaderDatas["shaders.vert"];
		vertSpvData.resize(vertFileSize);
		vertFile.Read(vertSpvData.data(), vertFileSize);
	}

	{
		File fragFile;
		if (!fragFile.Open(fragPath.c_str()))
			return false;

		uint32 fragFileSize = fragFile.GetSize();
		vector<uint32> &fragSpvData = m_shaderDatas["shaders.frag"];
		fragSpvData.resize(fragFileSize);
		fragFile.Read(fragSpvData.data(), fragFileSize);
	}

	return true;
}

bool VulkanShaderProgram::SetSourceWithPath(const char8 *sourcePath)
{
	return true;
}

bool VulkanShaderProgram::SetSource(const char8 *source)
{
	return true;
}

bool VulkanShaderProgram::CompileVertexFunctionWithName(const char8 *name)
{
	vector<uint32>& vertSpvData = m_shaderDatas[name];

	VkShaderModuleCreateInfo vert_info = {};
	vert_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vert_info.codeSize = vertSpvData.size();
	vert_info.pCode = vertSpvData.data();

	VkResult res = vkCreateShaderModule(m_device, &vert_info, NULL, &m_vertShader);
	if(res != VK_SUCCESS) return false;

	m_shaderStages[0].module = m_vertShader;

	return true;
}

bool VulkanShaderProgram::CompileFragmentFunctionWithName(const char8 *name)
{
	vector<uint32>& fragSpvData = m_shaderDatas[name];

	VkShaderModuleCreateInfo frag_info = {};
	frag_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	frag_info.codeSize = fragSpvData.size();
	frag_info.pCode = fragSpvData.data();

	VkResult res = vkCreateShaderModule(m_device, &frag_info, NULL, &m_fragShader);
	if(res != VK_SUCCESS) return false;

	m_shaderStages[1].module = m_fragShader;

	return true;
}
