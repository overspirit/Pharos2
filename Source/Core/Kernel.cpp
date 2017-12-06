#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
//#include "DesktopGlobal.h"
#include "SceneGlobal.h"

Kernel::Kernel()
{
	m_hWnd = nullptr;

	m_fontLib = nullptr;

	m_pApp = nullptr;
}

Kernel::~Kernel()
{
	FreeImage_DeInitialise();

	FT_Done_FreeType(m_fontLib);
}

bool Kernel::Init(const void* hWnd)
{
	m_hWnd = hWnd;

	FT_Init_FreeType(&m_fontLib);
	FreeImage_Initialise();

    //m_captureImage = MakeSharedPtr<Image>();
    //m_captureImage->CreateImage(1920, 1080);
    
	return true;
}

bool Kernel::StartUp()
{
	m_pApp = NewApplication();

	if (!m_pApp->Init()) return false;

	return true;
}

void Kernel::Destroy()
{
	for (File* file : m_fileList)
	{
		SAFE_DELETE(file);
	}

	//由于Kernel的析构函数会进行一些内存回收工作
	//所以要求Kernel持有的所有对象必须在Destroy方法内回收...
	m_pApp->Destroy();
	SAFE_DELETE(m_pApp);	//在这里析构IApplication对象，因为这个对象可能持有其他对象...	
}

void Kernel::SetCaptureOutput(const void* data, uint32 dataSize)
{
    //const void* captureData = m_captureImage->GetDataPointer();
    //memcpy((void*)captureData, data, dataSize);
}

void Kernel::onKeyboardEvent(const KeyEvent& keyEvent)
{
	if (m_pApp != nullptr) m_pApp->onKeyboardEvent(keyEvent);
}

void Kernel::onMouseEvent(const MouseEvent& mouseEvent)
{
	if (m_pApp != nullptr) m_pApp->onMouseEvent(mouseEvent);
}

void Kernel::onViewCreate()
{
	if (m_pApp != nullptr) m_pApp->onViewCreate();
}

void Kernel::onViewChangeSize(int32 width, int32 height)
{
	m_wndSize.width = width;
	m_wndSize.height = height;
	
	if (m_pApp != nullptr) m_pApp->onViewChangeSize(width, height);
}

void Kernel::onViewDestroy()
{
	if (m_pApp != nullptr) m_pApp->onViewDestroy();
}

void Kernel::Update(float32 fElapsed)
{
	if (m_pApp != nullptr) m_pApp->Update(fElapsed);
}

void Kernel::Render(float32 fElapsed)
{
	if (m_pApp != nullptr) m_pApp->Render(fElapsed);
}

File* Kernel::CreateFileStream(const char8* path, bool truncate)
{
	string fullPath = path;

	File* file = new File();
	if (file->Create(fullPath.c_str(), truncate))
	{
		m_fileList.push_back(file);
		return file;
	}

	fullPath = m_homePath + path;
	if (file->Create(fullPath.c_str(), truncate))
	{
		m_fileList.push_back(file);
		return file;
	}

	fullPath = m_bundlePath + path;
	if (file->Create(fullPath.c_str(), truncate))
	{
		m_fileList.push_back(file);
		return file;
	}

	SAFE_DELETE(file);
	return nullptr;
}

File* Kernel::OpenFileStream(const char8* path)
{
	string fullPath = path;
    if(fullPath.empty()) return nullptr;

	File* file = new File();
	if (file->Open(fullPath.c_str()))
	{
		m_fileList.push_back(file);
		return file;
	}

	fullPath = m_homePath + path;
	if (file->Open(fullPath.c_str()))
	{
		m_fileList.push_back(file);
		return file;
	}

	fullPath = m_bundlePath + path;
	if (file->Open(fullPath.c_str()))
	{
		m_fileList.push_back(file);
		return file;
	}

	SAFE_DELETE(file);
	return nullptr;
}
// 
// IXmlDocumentPtr Kernel::QueryXmlDocResource(const char8* path)
// {
// 	MyXmlDocumentPtr xmlDoc = MakeSharedPtr<XmlDocument>();
// 	if (!xmlDoc->Open(path)) return nullptr;
// 	if (!xmlDoc->Load()) return nullptr;
// 	return xmlDoc;
// }
// 
// IXmlDocumentPtr Kernel::CreateXmlDocResource(const char8* rootName)
// {
// 	MyXmlDocumentPtr xmlDoc = MakeSharedPtr<XmlDocument>();
// 	if (!xmlDoc->CreateRootNode(rootName)) return nullptr;
// 	return xmlDoc;
// }
// 
// IFontPtr Kernel::QueryFontResource(const char8* path)
// {
// 	FontPtr font = MakeSharedPtr<Font>(m_fontLib);
// 	if (!font->Open(path)) return nullptr;
// 	return font;
// }
// 
// IImagePtr Kernel::QueryImageResource(const char8* path)
// {
// 	ImagePtr image = MakeSharedPtr<Image>();
// 	if (!image->Open(path)) return nullptr;
// 	if (!image->LoadImageFile()) return nullptr;
// 	return image;
// }
// 
// IImagePtr Kernel::CreateImageResource(uint32 width, uint32 height)
// {
// 	ImagePtr image = MakeSharedPtr<Image>();
// 	if (!image->CreateImage(width, height)) return nullptr;
// 	return image;
// }
// 
// IMemBufferPtr Kernel::CreateMemBuffer(uint32 size)
// {
// 	return MakeSharedPtr<CMemoryBuffer>(size);
// }
// 
// ITimerPtr Kernel::CreateTimer(bool bStart)
// {
// 	ITimerPtr pTimer = MakeSharedPtr<Timer>();
// 	if (bStart) pTimer->Reset();
// 	return pTimer;
// }
// 
// IPropertiesPtr Kernel::QueryPropertiesResource(const char8* path)
// {
// 	PropertiesPtr prop = MakeSharedPtr<Properties>();
// 	if (!prop->Open(path)) return nullptr;
// 	return prop;
// }