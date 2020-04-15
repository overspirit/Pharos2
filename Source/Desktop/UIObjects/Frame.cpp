#include "PreCompile.h"
#include "Pharos.h"

Frame::Frame(void)
{
	m_type = "Frame";

	m_backDropTex = nullptr;

	m_bEnable = true;
	m_bDrawGray = false;
	m_bHidden = false;
}

Frame::~Frame(void)
{	
}

bool Frame::LoadFromXml(XmlNode* xmlNode)
{
	if (!UIObject::LoadFromXml(xmlNode)) return false;

	m_bEnable = GetAttributeBoolValue(xmlNode, "enable");
	m_bHidden = GetAttributeBoolValue(xmlNode, "hidden");
 
	m_backDropTex = GetTextureFromXmlNode(xmlNode, "BackDrop");
	m_backDropEdge = GetEdgeFromXmlNode(xmlNode, "BackDrop");

	XmlNode* framesNode = xmlNode->GetFirstNode("Frames");
	if (framesNode != nullptr)
	{
		for (XmlNode* childNode = framesNode->GetFirstNode(); childNode != nullptr; childNode = childNode->GetNextSibling())
		{
			UIObject* child = sDesktopMgr->GenerateUIObject(childNode, m_name.c_str());
			if (child != nullptr)
			{
				this->AddChild(child);
			}
		}
	}

	return true;
}

Rect2Di Frame::GetEdgeFromXmlNode(XmlNode* xmlNode, const char8* name)
{
	Rect2Di rect;

	XmlNode* styleNode = xmlNode->GetFirstNode(name);
	if (styleNode == nullptr) return rect;
	
	XmlNode* edgeNode = styleNode->GetFirstNode("EdgeInsets");
	if (edgeNode == nullptr) return rect;

	rect.left = GetAttributeIntValue(edgeNode, "left");
	rect.top = GetAttributeIntValue(edgeNode, "top");
	rect.right = GetAttributeIntValue(edgeNode, "right");
	rect.bottom = GetAttributeIntValue(edgeNode, "bottom");

	return rect;
}

Texture* Frame::GetTextureFromXmlNode(XmlNode* xmlNode, const char8* name)
{
	XmlNode* styleNode = xmlNode->GetFirstNode(name);
	if (styleNode == nullptr) return nullptr;

	XmlNode* texNode = styleNode->GetFirstNode("Texture");
	if (texNode == nullptr) return nullptr;

	return sDesktopMgr->GenerateUIObjectCastType<Texture>(texNode, m_name.c_str());
}

void Frame::AddChild(UIObject* child)
{
	if (child == nullptr) return;

	UIObject* parent = sDesktopMgr->GetControl(m_name.c_str());

	child->SetParent(parent);

	m_childList.push_back(child);
}

bool Frame::onInputMessage(const tagInputMsg& msg)
{
	if (m_bHidden || !m_bEnable) return false;

	bool result = false;

	switch(msg.type)
	{
	case EIT_LEFT_DOWN: result = onLeftButtonDown(msg); break;
	case EIT_LEFT_UP: result = onLeftButtonUp(msg); break;
	case EIT_MOUSE_MOVE: result = onMouseMove(msg); break;
	}

	for (auto obj : m_childList)
	{
		result = obj->onInputMessage(msg) || result;
	}

	return result;
}

bool Frame::onLeftButtonDown(const tagInputMsg& msg)
{
	return false;
}

bool Frame::onLeftButtonUp(const tagInputMsg& msg)
{
	return false;
}

bool Frame::onMouseMove(const tagInputMsg& msg)
{
	return false;
}

void Frame::Update(float32 fElapsed)
{
	if (m_backDropTex != nullptr)
	{
		m_backDropTex->Update(fElapsed);
	}

	UIObject::Update(fElapsed);

	for (auto obj : m_childList)
	{
		obj->Update(fElapsed);
	}
}

void Frame::Render(float32 fElapsed)
{	
	if (m_bHidden) return;

	if (m_backDropTex != nullptr)
	{
		m_backDropTex->Render(fElapsed);
	}

	UIObject::Render(fElapsed);

	for (auto obj : m_childList)
	{
		obj->Render(fElapsed);
	}
}
