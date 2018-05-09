#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"

Button::Button(void)
{
	m_type = "Button";

	m_btnState = EBS_PopUp;			//按钮状态
	m_isInsideDown = false;

	m_currColor = 0xFFFFFFFF;
}

Button::~Button(void)
{
}

bool Button::LoadFromXml(XmlNode* xmlNode)
{
	if (!Frame::LoadFromXml(xmlNode)) return false;

	m_pushTex = GetTextureFromXmlNode(xmlNode, "PushStyle");
	m_highlightTex = GetTextureFromXmlNode(xmlNode, "HighlightStyle");
	m_disableTex = GetTextureFromXmlNode(xmlNode, "DisableStyle");

	m_currTex = nullptr;

	return true;
}

void Button::Update(float32 fElapsed)
{
	if (m_currTex != nullptr)
	{
		m_currTex->Update(fElapsed);
	}

	Frame::Update(fElapsed);
}

void Button::Render(float32 fElapsed)
{
	Frame::Render(fElapsed);

	if (m_currTex != nullptr)
	{
		m_currTex->Render(fElapsed);
	}	
}

void Button::SetState(ButtonState state)
{
	m_btnState = state;

	switch (state)
	{
	case EBS_PopUp:
	{
		m_currTex = nullptr;
	}
	break;
	case EBS_PushDown:
	{
		m_currTex = m_pushTex;
	}
	break;
	case EBS_Highlight:
	{
		m_currTex = m_highlightTex;
	}
	break;
	case EBS_Disabled:
	{		
		m_currTex = m_disableTex;
	}
	break;
	default:
	{
		m_btnState = EBS_PopUp;
		m_currTex = nullptr;
	}
	}
}

void Button::Enable()
{
	if (m_rect.IsPointInside(m_currMouse))
	{
		SetState(EBS_Highlight);
	}
	else
	{
		SetState(EBS_PopUp);
	}
}

void Button::Disable()
{
	SetState(EBS_Disabled);
}

bool Button::onLeftButtonDown(const tagInputMsg& msg)
{
	if (!m_bEnable) return false;
    
	if (m_rect.IsPointInside(msg.p1, msg.p2))
	{
		//鼠标在控件中并且被按下就是按下状态
		SetState(EBS_PushDown);

		m_isInsideDown = true;
        
		return true;
	}
	else
	{
		m_isInsideDown = false;
	}
	return false;
}

bool Button::onLeftButtonUp(const tagInputMsg& msg)
{
	if (!m_bEnable) return false;
    
	if (m_rect.IsPointInside(msg.p1, msg.p2))
	{
		SetState(EBS_Highlight);
	}
	else
	{
		SetState(EBS_PopUp);
	}

	if (m_isInsideDown)
	{
		PushEvent(0, 0);
	}

	m_isInsideDown = false;
    
	return false;
}

bool Button::onMouseMove(const tagInputMsg& msg)
{
	m_currMouse.Set(msg.p1, msg.p2);

	if (!m_bEnable) return false;

	if (m_isInsideDown)
	{
		SetState(EBS_PushDown);
	}
	else
	{
		if (m_rect.IsPointInside(msg.p1, msg.p2))
		{
			SetState(EBS_Highlight);
		}
		else
		{
			SetState(EBS_PopUp);
		}
	}

	return false;
}
