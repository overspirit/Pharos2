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

	m_libPackage = nullptr;
	
	m_vertShader = VK_NULL_HANDLE;
	m_fragShader = VK_NULL_HANDLE;
}

VulkanShaderProgram::~VulkanShaderProgram()
{
	vkDestroyShaderModule(m_device, m_vertShader, NULL);
	vkDestroyShaderModule(m_device, m_fragShader, NULL);
}

bool VulkanShaderProgram::SetLibraryWithPath(const char8* libPath)
{
	m_libPackage = sResMgr->GeneratePackage(libPath);
	if (m_libPackage == nullptr) return false;

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
	string funcFileName = string(name) + ".spv";

	MemoryBuffer* vertSpvData = m_libPackage->GetPackageFileBuffer(funcFileName.c_str());
	if (vertSpvData == nullptr) return false;

	VkShaderModuleCreateInfo vert_info = {};
	vert_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vert_info.codeSize = vertSpvData->GetLength();
	vert_info.pCode = (uint32*)vertSpvData->GetPointer();

	VkResult res = vkCreateShaderModule(m_device, &vert_info, NULL, &m_vertShader);
	if(res != VK_SUCCESS) return false;

	m_shaderStages[0].module = m_vertShader;

	return true;
}

bool VulkanShaderProgram::CompileFragmentFunctionWithName(const char8 *name)
{
	string funcFileName = string(name) + ".spv";

	MemoryBuffer* fragSpvData = m_libPackage->GetPackageFileBuffer(funcFileName.c_str());
	if (fragSpvData == nullptr) return false;

	VkShaderModuleCreateInfo frag_info = {};
	frag_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	frag_info.codeSize = fragSpvData->GetLength();
	frag_info.pCode = (uint32*)fragSpvData->GetPointer();

	VkResult res = vkCreateShaderModule(m_device, &frag_info, NULL, &m_fragShader);
	if(res != VK_SUCCESS) return false;

	m_shaderStages[1].module = m_fragShader;

	return true;
}
