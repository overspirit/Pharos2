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

bool VulkanShaderProgram::SetLibraryWithPath(const char8* libPath)
{
	string fullLibPath = sResMgr->FindResourcePath(libPath);
	if (fullLibPath.empty()) return false;

	unzFile lib_file = unzOpen64(fullLibPath.c_str());
	if (lib_file == NULL) return false;

	unz_global_info64 global_info;
	if (unzGetGlobalInfo64(lib_file, &global_info) == UNZ_OK)
	{
		for (int i = 0; i < global_info.number_entry; i++)
		{
			unz_file_info64 file_info;
			char file_name[MAX_PATH] = {0};
			if(unzGetCurrentFileInfo64(lib_file, &file_info, file_name, sizeof(file_name), NULL, 0, NULL, 0) != UNZ_OK)
			{
				break;
			}

			if(unzOpenCurrentFile(lib_file) != UNZ_OK) break;

			Utils::Path path(file_name);
			string shaderName = path.GetFileName();

			vector<uint32> &spvData = m_shaderDatas[shaderName];
			spvData.resize(file_info.uncompressed_size);
			unzReadCurrentFile(lib_file, spvData.data(), file_info.uncompressed_size);

			unzCloseCurrentFile(lib_file);
			unzGoToNextFile(lib_file);
		}
	}

	unzClose(lib_file);

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
