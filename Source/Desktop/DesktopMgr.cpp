#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"

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
}

DesktopMgr::~DesktopMgr()
{

}

bool DesktopMgr::Init()
{
	m_worldFrame = MakeSharedPtr<WorldFrame>();
	m_worldFrame->Init();

	return true;
}

void DesktopMgr::Destroy()
{
	m_worldFrame = nullptr;

	m_frameList.clear();
	m_controlList.clear();
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
	XmlDocument doc;
	if (!doc.Open(szFile)) return false;
	if (!doc.Load()) return false;

	XmlNode* pRoot = doc.GetRootNode();
	if (pRoot == nullptr) return false;

	for (uint32 i = 0; i < pRoot->GetChildNum(); i++)
	{
		XmlNode* childNode = pRoot->GetChildNode(i);
		UIObjectPtr uiObj = this->GenerateUIObject(childNode, "");
		if (uiObj != nullptr)
		{
			m_worldFrame->AddChild(uiObj);
		}
	}

	return true;
}

bool DesktopMgr::RegisterControlViewer(IControlViewer* viewer)
{
	if (viewer == nullptr) return false;

	m_viewerList.push_back(viewer);

	return true;
}

void DesktopMgr::ReceivedEvent(const char8* name, int32 v1, float32 v2)
{
	for (IControlViewer* viewer : m_viewerList)
	{
		viewer->onControlValueChange(name, v1, v2);
	}
}

UIObjectPtr DesktopMgr::GenerateUIObject(XmlNode* xmlNode, const char8* parentName)
{
	if (xmlNode == nullptr) return nullptr;

	const char8* type = xmlNode->GetName();
	
	string name;

	XmlAttribute* nameAttr = xmlNode->GetAttribute("name");
	if (nameAttr != nullptr) name = nameAttr->GetStringValue();

	UIObjectPtr uiObj;

	if (!name.empty())		//名字不为空就要判断是否存在这个UIObject，不允许有重名
	{
		uiObj = GetControl(name.c_str());
		if (uiObj != nullptr) return nullptr;	//该名字已经使用

		uiObj = sUIFactroy.CreateObject(type);
		if (uiObj == nullptr) return nullptr;

		m_frameList[name] = uiObj;
	}
	else   //名字为空就进入m_controlList，因为所有parent和child都使用weak_ptr, 不保存列表就会被释放
	{
		uiObj = sUIFactroy.CreateObject(type);
		if (uiObj == nullptr) return nullptr;

		m_controlList.push_back(uiObj);
	}

	UIObjectPtr parent = GetControl(parentName);
	if (parent == nullptr) parent = m_worldFrame;
	uiObj->SetParent(parent);

	if (!uiObj->LoadFromXml(xmlNode)) return nullptr;	//LoadFromXml函数中调用GenerateUIObject	

	return uiObj;
}

UIObjectPtr DesktopMgr::GetControl(const char8* szName)
{
	map<string, UIObjectPtr>::iterator iter = m_frameList.find(szName);
	return (iter != m_frameList.end()) ? iter->second : nullptr;
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

void DesktopMgr::onViewChangeSize(int32 width, int32 height)
{
	if (m_worldFrame == nullptr) return;

	const Size2Di& desktopSize = m_worldFrame->GetSize();
	if (desktopSize.IsEmpty()) return;

	m_fScaleX = (float32)desktopSize.width / width;
	m_fScaleY = (float32)desktopSize.height / height;
}

void DesktopMgr::Update(float32 fElapsed)
{
	m_worldFrame->Update(fElapsed);
}

void DesktopMgr::Render(float32 fElapsed)
{
	//IFrameBuffer* defFrameBuf = g_pDevice->GetDefaultFrameBuffer();
	//defFrameBuf->ClearFrameBuffer(0x00, 1.0f, 0);

	//g_pDevice->SetFrameBuffer(defFrameBuf);

	m_worldFrame->Render(fElapsed);

	sRenderSpirite->PostSubmitBlock();
}