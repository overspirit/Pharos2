#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"
#include "SceneGlobal.h"

#pragma message("------------------------------------资源的异步加载未完成!!!------------------------------------")
#pragma message("------------------------------------资源的引用计数未完成!!!------------------------------------")

ResourceManager::ResourceManager()
{
	m_fontLib = nullptr;
}

ResourceManager::~ResourceManager()
{

}

bool ResourceManager::Init()
{
	FT_Init_FreeType(&m_fontLib);
	FreeImage_Initialise();

	m_currWorkPath = sKernel->GetBundleDirectoryPath();

	return true;
}

void ResourceManager::Destroy()
{
	for (auto iter = m_storageResList.begin(); iter != m_storageResList.end(); iter++)
	{
		ResBase* res = iter->second;
		SAFE_DELETE(res);
	}

	for (uint32 i = 0; i < m_pendingResList.size(); i++)
	{
		SAFE_DELETE(m_pendingResList[i]);
	}

	FreeImage_DeInitialise();

	FT_Done_FreeType(m_fontLib);
}

ResBase* ResourceManager::GenerateResource(ResType resType)
{
	ResBase* res = nullptr;

	switch (resType)
	{
		case ERT_FONT: res = new Font(m_fontLib); break;
		case ERT_IMAGE: res = new Image(); break;
		case ERT_XML: res = new XmlDocument(); break;
		default: return nullptr;
	}

	if (res != nullptr)
	{
		m_pendingResList.push_back(res);
	}

	return res;
}

string ResourceManager::FindResourcePath(const char8* path)
{
	if (access(path, 0) == 0)
	{
		return path;
	}

	Utils::Path pendingPath(path);
	string pendingDir = pendingPath.GetPathDirectory();

	if (pendingDir.empty())
	{
		string fullPath = m_currWorkPath + path;
		if (access(fullPath.c_str(), 0) == 0)
		{
			return fullPath;
		}
	}
	else
	{
		string homePath = sKernel->GetHomeDirectoryPath();
		string fullPath = homePath + path;
		if (access(fullPath.c_str(), 0) == 0)
		{
			Utils::Path tmp_path(fullPath.c_str());
			m_currWorkPath = tmp_path.GetFullPath();
			return fullPath;
		}

		string bundlePath = sKernel->GetBundleDirectoryPath();
		fullPath = bundlePath + path;
		if (access(fullPath.c_str(), 0) == 0)
		{
			Utils::Path tmp_path(fullPath.c_str());
			m_currWorkPath = tmp_path.GetFullPath();
			return fullPath;
		}
	}

	return path;
}

Font* ResourceManager::GenerateFont(const char8* path)
{
	auto iter = m_storageResList.find(path);
	if (iter != m_storageResList.end())
	{
		ResBase* res = iter->second;
		ResType resType = res->GetResType();
		assert(resType == ERT_FONT);
		return (Font*)res;
	}
	else
	{
		Font* font = new Font(m_fontLib);
		const string& absPath = FindResourcePath(path);
		if (!font->Open(absPath.c_str()))
		{
			SAFE_DELETE(font);
			return nullptr;
		}

		m_storageResList[path] = font;
		return font;
	}

	return nullptr;
}

Image* ResourceManager::GenerateImage(const char8* path)
{
	auto iter = m_storageResList.find(path);
	if (iter != m_storageResList.end())
	{
		ResBase* res = iter->second;
		ResType resType = res->GetResType();
		assert(resType == ERT_IMAGE);
		return (Image*)res;
	}
	else
	{
		Image* image = new Image();
		const string& absPath = FindResourcePath(path);
		if (!image->Open(absPath.c_str()))
		{
			SAFE_DELETE(image);
			return nullptr;
		}

		m_storageResList[path] = image;
		return image;
	}

	return nullptr;
}

XmlDocument* ResourceManager::GenerateXmlDocument(const char8* path)
{
	auto iter = m_storageResList.find(path);
	if (iter != m_storageResList.end())
	{
		ResBase* res = iter->second;
		ResType resType = res->GetResType();
		assert(resType == ERT_XML);
		return (XmlDocument*)res;
	}
	else
	{
		XmlDocument* doc = new XmlDocument();
		const string& absPath = FindResourcePath(path);
		if (!doc->Open(absPath.c_str()))
		{
			SAFE_DELETE(doc);
			return nullptr;
		}

		m_storageResList[path] = doc;
		return doc;
	}

	return nullptr;
}