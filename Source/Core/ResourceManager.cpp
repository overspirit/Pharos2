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
	
	m_storageResList.clear();

	for (uint32 i = 0; i < m_pendingResList.size(); i++)
	{
		SAFE_DELETE(m_pendingResList[i]);
	}

	m_pendingResList.clear();

	FreeImage_DeInitialise();

	FT_Done_FreeType(m_fontLib);
}

File* ResourceManager::QueryResourceHandler(const char8* path, bool opened)
{
	if (path == nullptr || path[0] == '\0') return nullptr;
	
	File* file = new File();	

    string checkPathes[] = {
        m_currWorkPath + path,
        string(sKernel->GetHomeDirectoryPath()) + path,
        string(sKernel->GetBundleDirectoryPath()) + path,
        path
    };
    
    for (const string& checkPath : checkPathes)
    {
        bool ret = false;
        if (opened) ret = file->Open(checkPath.c_str());
        else ret = file->Create(checkPath.c_str(), true);
        
        if (ret)
        {
            Utils::Path tmp_path(checkPath.c_str());
            m_currWorkPath = tmp_path.GetFullPath();

            return file;
        }
    }
    
	SAFE_DELETE(file);
	return nullptr;
}


ResBase* ResourceManager::GenerateResource(ResType resType, const char8* path)
{
    auto iter = m_storageResList.find(path);
    if (iter != m_storageResList.end())
    {
        ResBase* res = iter->second;
        assert(res->GetResType() == resType);
        return res;
    }
    
    File* file = QueryResourceHandler(path, true);
    if (file == NULL)
    {
        return NULL;
    }

    ResBase* res = nullptr;
    switch (resType)
    {
        case ERT_FONT: res = new Font(m_fontLib); break;
        case ERT_IMAGE: res = new Image(); break;
        case ERT_XML: res = new XmlDocument(); break;
        case ERT_PACKAGE: res = new Package(); break;
    }
    
    if (res == nullptr)
    {
        SAFE_DELETE(file);
        return res;
    }

    if (!res->Open(file))
    {
        SAFE_DELETE(file);
        SAFE_DELETE(res);
        return nullptr;
    }
    
    m_storageResList[path] = res;

    SAFE_DELETE(file);
	return res;
}

ResBase* ResourceManager::CreateResource(ResType resType, const char8* path)
{
    auto iter = m_storageResList.find(path);
    if (iter != m_storageResList.end())
    {
        ResBase* res = iter->second;
        assert(res->GetResType() == resType);
        return res;
    }
    
    File* file = QueryResourceHandler(path, false);
    if (file == NULL)
    {
        return NULL;
    }

    ResBase* res = nullptr;
    switch (resType)
    {
        case ERT_FONT: res = new Font(m_fontLib); break;
        case ERT_IMAGE: res = new Image(); break;
        case ERT_XML: res = new XmlDocument(); break;
        case ERT_PACKAGE: res = new Package(); break;
    }
    
    if (res == nullptr)
    {
        SAFE_DELETE(file);
        return res;
    }

    if (!res->Create(file))
    {
        SAFE_DELETE(file);
        SAFE_DELETE(res);
        return nullptr;
    }
    
    m_pendingResList.push_back(res);

    SAFE_DELETE(file);
    return res;
}

Font* ResourceManager::GenerateFont(const char8* path)
{
    return (Font*)GenerateResource(ERT_FONT, path);;
}

Image* ResourceManager::GenerateImage(const char8* path)
{
    return (Image*)GenerateResource(ERT_IMAGE, path);
}

XmlDocument* ResourceManager::GenerateXmlDocument(const char8* path)
{
    return (XmlDocument*)GenerateResource(ERT_XML, path);
}

Package* ResourceManager::GeneratePackage(const char8* path)
{
    return (Package*)GenerateResource(ERT_PACKAGE, path);
}
