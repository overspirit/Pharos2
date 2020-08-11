#include "PreCompile.h"
#include "Pharos.h"

//放在这里才会进入UIFactory，否则由于没有这几个类的实例所以会被编译器优化掉
IMPLEMENT_UI_CLASS(Button)
IMPLEMENT_UI_CLASS(CheckButton)
IMPLEMENT_UI_CLASS(Frame)
IMPLEMENT_UI_CLASS(FontString)
IMPLEMENT_UI_CLASS(Progress)
IMPLEMENT_UI_CLASS(Slider)
IMPLEMENT_UI_CLASS(Texture)

DesktopMgr::DesktopMgr()
{
	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;

	m_worldFrame = nullptr;
}

DesktopMgr::~DesktopMgr()
{

}

bool DesktopMgr::Init()
{
	m_worldFrame = new WorldFrame();
	m_worldFrame->Init();

	sRenderSpirite->Init();

	return true;
}

void DesktopMgr::Destroy()
{
	SAFE_DELETE(m_worldFrame);

	for (auto frame : m_frameList)
	{
		SAFE_DELETE(frame.second);
	}

	m_frameList.clear();

	for (auto control : m_controlList)
	{
		SAFE_DELETE(control);
	}

	m_controlList.clear();

	sFontTexMgr->DestroyAll();
}

void DesktopMgr::SetDesktopSize(int32 width, int32 height)
{
	m_worldFrame->SetSize(width, height);

	const Size2Di& wndSize = sKernel->GetWindowSize();
	m_fScaleX = (float32)width / wndSize.width;
	m_fScaleY = (float32)height / wndSize.height;
}

bool DesktopMgr::LoadUILayoutFile(const char8* szFile)
{
	XmlDocument* doc = sResMgr->GenerateXmlDocument(szFile);
	if (doc == nullptr) return false;

	XmlNode* pRoot = doc->GetRootNode();
	if (pRoot == nullptr) return false;

	for (uint32 i = 0; i < pRoot->GetChildNum(); i++)
	{
		XmlNode* childNode = pRoot->GetChildNode(i);
		UIObject* uiObj = this->GenerateUIObject(childNode, "");
		if (uiObj != nullptr)
		{
			m_worldFrame->AddChild(uiObj);
		}
	}

	return true;
}

bool DesktopMgr::RegisterControlViewer(const char8* uiName, IControlViewer* viewer, EVENT_CALLBACK callback)
{
	if (viewer == nullptr) return false;

	UIObject* uiObj = GetControl(uiName);
	if (uiObj == nullptr) return false;

	uiObj->AttachEventCallback(viewer, callback);

	return true;
}

UIObject* DesktopMgr::GenerateUIObject(XmlNode* xmlNode, const char8* parentName)
{
	if (xmlNode == nullptr) return nullptr;

	const char8* type = xmlNode->GetName();

	string name;

	XmlAttribute* nameAttr = xmlNode->GetAttribute("name");
	if (nameAttr != nullptr) name = nameAttr->GetStringValue();

	UIObject* uiObj = nullptr;

	if (!name.empty())		//名字不为空就要判断是否存在这个UIObject，不允许有重名
	{
		uiObj = GetControl(name.c_str());
		if (uiObj != nullptr) return nullptr;	//该名字已经使用

		uiObj = sUIFactroy.CreateObject(type);
		if (uiObj == nullptr) return nullptr;

		m_frameList[name] = uiObj;
	}
	else   //名字为空就进入m_controlList
	{
		uiObj = sUIFactroy.CreateObject(type);
		if (uiObj == nullptr) return nullptr;

		m_controlList.push_back(uiObj);
	}

	UIObject* parent = GetControl(parentName);
	if (parent == nullptr) parent = m_worldFrame;
	uiObj->SetParent(parent);

	if (!uiObj->LoadFromXml(xmlNode)) return nullptr;	//LoadFromXml函数中调用GenerateUIObject	

	return uiObj;
}

UIObject* DesktopMgr::GetControl(const char8* szName)
{
	map<string, UIObject*>::iterator iter = m_frameList.find(szName);
	return (iter != m_frameList.end()) ? iter->second : nullptr;
}

RenderFont* DesktopMgr::GenerateRenderFont(const char8* fontFilePath)
{
	RenderFont* renderFont = new RenderFont();
	if (renderFont->LoadFont(fontFilePath))
	{
		return renderFont;
	}

	SAFE_DELETE(renderFont);
	return nullptr;
}

RenderImage* DesktopMgr::GenerateRenderImage(const char8* imageFilePath)
{
	RenderImage* renderImage = new RenderImage();
	if (renderImage->LoadImage(imageFilePath))
	{
		return renderImage;
	}

	SAFE_DELETE(renderImage);
	return nullptr;
}

bool DesktopMgr::onMouseEvent(const MouseEvent& e)
{
	if (m_worldFrame == nullptr) return false;

	Desktop::tagInputMsg msg;

	switch (e.state)
	{
		case STATE_KEEP: msg.type = Desktop::EIT_MOUSE_MOVE; break;
		case STATE_DOWN:
		{
			switch (e.button)
			{
				case MOUSE_LEFT: msg.type = Desktop::EIT_LEFT_DOWN; break;
			}
		}
		break;
		case STATE_UP:
		{
			switch (e.button)
			{
				case MOUSE_LEFT: msg.type = Desktop::EIT_LEFT_UP; break;
			}
		}
		break;
	}

	msg.p1 = (int32)(e.x * m_fScaleX);
	msg.p2 = (int32)(e.y * m_fScaleY);
	msg.p3 = (int32)(e.ox * m_fScaleX);
	msg.p4 = (int32)(e.oy * m_fScaleY);

	return m_worldFrame->onInputMessage(msg);
}

bool DesktopMgr::onKeyboardEvent(const KeyEvent& e)
{
	//if (m_worldFrame != nullptr) return m_worldFrame->onKeyboardEvent(e);

	return false;
}

void DesktopMgr::onViewCreate()
{

}

void DesktopMgr::onViewChangeSize(int32 width, int32 height)
{
	if (m_worldFrame == nullptr) return;

	const Size2Di& desktopSize = m_worldFrame->GetSize();
	if (desktopSize.IsEmpty()) return;

	m_fScaleX = (float32)desktopSize.width / width;
	m_fScaleY = (float32)desktopSize.height / height;
}

void DesktopMgr::onViewDestroy()
{

}

void DesktopMgr::Update(float32 fElapsed)
{
	if (m_worldFrame != nullptr) m_worldFrame->Update(fElapsed);
}

void DesktopMgr::Render(float32 fElapsed)
{
	if (m_worldFrame == nullptr) return;

	//IFrameBuffer* defFrameBuf = g_pDevice->GetDefaultFrameBuffer();
	//defFrameBuf->ClearFrameBuffer(0x00, 1.0f, 0);

	//g_pDevice->SetFrameBuffer(defFrameBuf);

	m_worldFrame->Render(fElapsed);

	//m_renderLayout->CopyVertexBuffer(&m_layoutBuffer, m_vertCount * sizeof(DecalColorVertex));

	//sRenderMgr->DoRender(m_renderBlock);

	sRenderSpirite->Render();
}
