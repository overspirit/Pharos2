#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"

UIObject::UIObject(void)
{
	m_type = "UIObject";

	m_parent = nullptr;
}

UIObject::~UIObject(void)
{

}
bool UIObject::LoadFromXml(XmlNode* xmlNode)
{
	m_name = GetAttributeStringValue(xmlNode, "name");

	XmlNode* regionNode = xmlNode->GetFirstNode("Region");
	if (regionNode == nullptr)
	{
		tagAnchor leftTopAnchor;
		leftTopAnchor.point = EAT_TopLeft;
		leftTopAnchor.relativeTo = m_parent;		
		leftTopAnchor.relativePoint = EAT_TopLeft;
		m_anchorList.push_back(leftTopAnchor);

		tagAnchor rightTopAnchor;
		rightTopAnchor.point = EAT_TopRight;
		rightTopAnchor.relativeTo = m_parent;
		rightTopAnchor.relativePoint = EAT_TopRight;
		m_anchorList.push_back(rightTopAnchor);

		tagAnchor leftBottomAnchor;
		leftBottomAnchor.point = EAT_BottomLeft;
		leftBottomAnchor.relativeTo = m_parent;
		leftBottomAnchor.relativePoint = EAT_BottomLeft;
		m_anchorList.push_back(leftBottomAnchor);

		tagAnchor rightBottomAnchor;
		rightBottomAnchor.point = EAT_BottomRight;
		rightBottomAnchor.relativeTo = m_parent;
		rightBottomAnchor.relativePoint = EAT_BottomRight;
		m_anchorList.push_back(rightBottomAnchor);
	}
	else
	{
		XmlNode* sizeNode = regionNode->GetFirstNode("Size");
		if (sizeNode != nullptr)
		{
			m_size = LoadSize(sizeNode);
		}

		for (XmlNode* anchorNode = regionNode->GetFirstNode("Anchor"); anchorNode != nullptr; anchorNode = anchorNode->GetNextSibling("Anchor"))
		{
			tagAnchor anchor = LoadAnchor(anchorNode);
			m_anchorList.push_back(anchor);
		}
	}

	return true;
}

bool UIObject::onInputMessage(const tagInputMsg& msg)
{
	//不处理消息，返回false
	return false;
}

void UIObject::DoEvent(const EventArgs& eventArgs)
{
	for (auto callback : m_callbackList)
	{
		IControlViewer* viewer = callback.first;
		EVENT_CALLBACK func = callback.second;

		(viewer->*func)(this, eventArgs);
	}
}

void UIObject::AttachEventCallback(IControlViewer* viewer, EVENT_CALLBACK callback)
{
	m_callbackList.push_back(std::make_pair(viewer, callback));
}

const char8* UIObject::GetAttributeStringValue(XmlNode* node, const char8* name)
{
	XmlAttribute* attr = node->GetAttribute(name);
	if (attr == nullptr) return "";

	return attr->GetStringValue();
}

int32 UIObject::GetAttributeIntValue(XmlNode* node, const char8* name)
{
	XmlAttribute* attr = node->GetAttribute(name);
	if (attr == nullptr) return -1;

	return attr->GetIntValue();
}

bool UIObject::GetAttributeBoolValue(XmlNode* node, const char8* name)
{
	XmlAttribute* attr = node->GetAttribute(name);
	if (attr == nullptr) return false;

	return attr->GetBoolValue();
}

float32 UIObject::GetAttributeFloatValue(XmlNode* node, const char8* name)
{
	XmlAttribute* attr = node->GetAttribute(name);
	if (attr == nullptr) return 0;

	return attr->GetFloatValue();
}

Color4 UIObject::GetAttributeColorValue(XmlNode* node, const char8* name)
{
	XmlAttribute* attr = node->GetAttribute(name);
	if (attr == nullptr) return 0xFFFFFFFF;

	return attr->GetColorValue();
}

//读取锚点
tagAnchor UIObject::LoadAnchor(XmlNode* xmlNode)
{
	tagAnchor anchor;

	const char8* point = GetAttributeStringValue(xmlNode, "point");
	anchor.point = GetAnchorType(point);

	const char8* name = GetAttributeStringValue(xmlNode, "relativeTo");
	UIObject* relativeTo = DesktopMgr::Inst()->GetControl(name);
	if (relativeTo == nullptr)
	{
		anchor.relativeTo = m_parent;
	}
	else
	{
		anchor.relativeTo = relativeTo;
	}

	const char8* relativePoint = GetAttributeStringValue(xmlNode, "relativePoint");
	anchor.relativePoint = GetAnchorType(relativePoint);

	//如果没有设置参考控件的锚点，即默认参考控件的锚点和锚点一样
	if (anchor.relativePoint == EAT_End) anchor.relativePoint = anchor.point;

	XmlNode* offsetNode = xmlNode->GetFirstNode("Offset");
	if (offsetNode != nullptr)
	{
		anchor.offset = LoadSize(offsetNode);
	}

	m_anchorList.push_back(anchor);

	return anchor;
}

Size2Di UIObject::LoadSize(XmlNode* xmlNode)
{
	Size2Di size;
	size.width = GetAttributeIntValue(xmlNode, "x");
	size.height = GetAttributeIntValue(xmlNode, "y");
	return size;
}

void UIObject::Update(float32 fElapsed)
{
	m_rect.left = 0x7FFFFFFF;
	m_rect.top = 0x7FFFFFFF;
	m_rect.right = 0x7FFFFFFF;
	m_rect.bottom = 0x7FFFFFFF;

	m_center.x = 0x7FFFFFFF;
	m_center.y = 0x7FFFFFFF;
	
	//计算m_rect
	for (auto& anchor : m_anchorList)
	{
		UIObject* relativeTo = anchor.relativeTo;
		const Rect2Di& relativeRect = relativeTo->GetRect();
		const Point2Di relativeRectCenter = relativeRect.GetCenter();
		const Size2Di& offset = anchor.offset;
		int32 topBottom = 0;
		int32 leftRight = 0;

		switch (anchor.relativePoint)
		{
		case EAT_TopLeft:		leftRight = relativeRect.left + offset.width;			topBottom = relativeRect.top + offset.height;			break;
		case EAT_Top:																	topBottom = relativeRect.top + offset.height;			break;
		case EAT_TopRight:		leftRight = relativeRect.right + offset.width;			topBottom = relativeRect.top + offset.height;			break;
		case EAT_Left:			leftRight = relativeRect.left + offset.width;																	break;
		case EAT_Center:		leftRight = relativeRectCenter.x + offset.width;		topBottom = relativeRectCenter.y + offset.height;	 	break;
		case EAT_Right:			leftRight = relativeRect.right + offset.width;																	break;
		case EAT_BottomLeft:	leftRight = relativeRect.left + offset.width;			topBottom = relativeRect.bottom + offset.height;		break;
		case EAT_Bottom:																topBottom = relativeRect.bottom + offset.height;		break;
		case EAT_BottomRight:	leftRight = relativeRect.right + offset.width;			topBottom = relativeRect.bottom + offset.height;		break;
		}

		switch (anchor.point)
		{
		case EAT_TopLeft:		m_rect.left = leftRight;		m_rect.top = topBottom;			break;
		case EAT_Top:											m_rect.top = topBottom;			break;
		case EAT_TopRight:		m_rect.right = leftRight;		m_rect.top = topBottom;			break;
		case EAT_Left:			m_rect.left = leftRight;										break;
		case EAT_Center:		m_center.x = leftRight;			m_center.y = topBottom;			break;
		case EAT_Right:			m_rect.right = leftRight;										break;
		case EAT_BottomLeft:	m_rect.left = leftRight;		m_rect.bottom = topBottom;		break;
		case EAT_Bottom:										m_rect.bottom = topBottom;		break;
		case EAT_BottomRight:	m_rect.right = leftRight;		m_rect.bottom = topBottom;		break;
		}
	}

	if (m_rect.left == 0x7FFFFFFF)
	{
		if (m_rect.right != 0x7FFFFFFF) m_rect.left = m_rect.right - m_size.width;
		else if (m_center.x != 0x7FFFFFFF) m_rect.left = m_center.x - m_size.width / 2;
		else m_rect.left = 0;
	}

	if (m_rect.top == 0x7FFFFFFF)
	{
		if (m_rect.bottom != 0x7FFFFFFF) m_rect.top = m_rect.bottom - m_size.height;
		else if (m_center.y != 0x7FFFFFFF) m_rect.top = m_center.y - m_size.height / 2;
		else m_rect.top = 0;
	}

	if (m_rect.right == 0x7FFFFFFF) m_rect.right = m_rect.left + m_size.width;
	if (m_rect.bottom == 0x7FFFFFFF) m_rect.bottom = m_rect.top + m_size.height;

	m_size = m_rect.GetSize();
	m_center = m_rect.GetCenter();
}

void UIObject::Render(float32 fElapsed)
{
	//sRenderSpirite->RenderWireFrame(0xFFDF932B, Rect2Di(m_rect.left - 1, m_rect.top - 1, m_rect.right + 1, m_rect.bottom + 1));
}
