#include "PreCompile.h"
#include "Pharos.h"

//#pragma message("------------------------------------资源的异步加载未完成!!!------------------------------------")
//#pragma message("------------------------------------资源的引用计数未完成!!!------------------------------------")

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

File* ResourceManager::CreateResourceFile(const char8* path)
{
	File* file = new File();	

	Utils::Path pendingPath(path);
	string pendingDir = pendingPath.GetPathDirectory();

	if (pendingDir.empty())
	{
		string fullPath = m_currWorkPath + path;
		if (file->Open(fullPath.c_str()))
		{
			return file;
		}
	}
	else
	{
		string homePath = sKernel->GetHomeDirectoryPath();
		string fullPath = homePath + path;
		if (file->Open(fullPath.c_str()))
		{
			Utils::Path tmp_path(fullPath.c_str());
			m_currWorkPath = tmp_path.GetFullPath();
			return file;
		}

		string bundlePath = sKernel->GetBundleDirectoryPath();
		fullPath = bundlePath + path;
		if (file->Open(fullPath.c_str()))
		{
			Utils::Path tmp_path(fullPath.c_str());
			m_currWorkPath = tmp_path.GetFullPath();
			return file;
		}
	}

	SAFE_DELETE(file);
	return nullptr;
}

ResBase* ResourceManager::FindResource(const char8* path, ResType type)
{
	auto iter = m_storageResList.find(path);
	if (iter != m_storageResList.end())
	{
		ResBase* res = iter->second;
		ResType resType = res->GetResType();
		assert(resType == type);
		return res;
	}

	return NULL;
}

ResBase* ResourceManager::GenerateResource(ResType resType, File* file)
{
	ResBase* res = nullptr;

	switch (resType)
	{
		case ERT_FONT: res = new Font(m_fontLib); break;
		case ERT_IMAGE: res = new Image(); break;
		case ERT_XML: res = new XmlDocument(); break;
		case ERT_PACKAGE: res = new Package(); break;
		default: return nullptr;
	}

	if (file == nullptr)
	{
		m_pendingResList.push_back(res);
	}
	else
	{
		if (!res->Open(file))
		{
			SAFE_DELETE(res);
			return nullptr;
		}

		const char8* path = file->GetPath();
		m_storageResList[path] = res;
	}
	

	return res;
}

Font* ResourceManager::GenerateFont(const char8* path)
{
	ResBase* res = FindResource(path, ERT_FONT);
	if (res != nullptr)
	{
		return (Font*)res;
	}

	File* resFile = CreateResourceFile(path);
	if (resFile == nullptr) return nullptr;
	
	res = GenerateResource(ERT_FONT, resFile);

	SAFE_DELETE(resFile);

	return (Font*)res;
}

Image* ResourceManager::GenerateImage(const char8* path)
{
	ResBase* res = FindResource(path, ERT_IMAGE);
	if (res != nullptr)
	{
		return (Image*)res;
	}

	File* resFile = CreateResourceFile(path);
	if (resFile == nullptr) return nullptr;

	res = GenerateResource(ERT_IMAGE, resFile);

	SAFE_DELETE(resFile);

	return (Image*)res;
}

XmlDocument* ResourceManager::GenerateXmlDocument(const char8* path)
{
	ResBase* res = FindResource(path, ERT_XML);
	if (res != nullptr)
	{
		return (XmlDocument*)res;
	}

	File* resFile = CreateResourceFile(path);
	if (resFile == nullptr) return nullptr;

	res = GenerateResource(ERT_XML, resFile);

	SAFE_DELETE(resFile);

	return (XmlDocument*)res;
}

Package* ResourceManager::GeneratePackage(const char8* path)
{
	ResBase* res = FindResource(path, ERT_PACKAGE);
	if (res != nullptr)
	{
		return (Package*)res;
	}

	File* resFile = CreateResourceFile(path);
	if (resFile == nullptr) return nullptr;

	res = GenerateResource(ERT_PACKAGE, resFile);

	SAFE_DELETE(resFile);

	return (Package*)res;
}