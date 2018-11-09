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
		if (!font->Open(path))
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
		if (!image->Open(path))
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
		if (!doc->Open(path))
		{
			SAFE_DELETE(doc);
			return nullptr;
		}

		m_storageResList[path] = doc;
		return doc;
	}

	return nullptr;
}