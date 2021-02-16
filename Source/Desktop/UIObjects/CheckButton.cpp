#include "PreCompile.h"
#include "Pharos.h"

CheckButton::CheckButton(void)
{
	m_type = "CheckButton";

	m_isCheck = false;

	m_checkTex = nullptr;
}

CheckButton::~CheckButton(void)
{

}

bool CheckButton::LoadFromXml(XmlNode* xmlNode)
{
	if (!Button::LoadFromXml(xmlNode)) return false;

	m_checkTex = GetTextureFromXmlNode(xmlNode, "CheckStyle");

	return true;
}

bool CheckButton::onLeftButtonUp(const tagInputMsg& msg)
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
		m_isCheck = !m_isCheck;

		EventArgs eventArgs(EventType::Click);
		eventArgs.argNum = 1;
		eventArgs.args[0].ba = m_isCheck;
		DoEvent(eventArgs);
	}

	m_isInsideDown = false;

	return false;
}

void CheckButton::Update(float32 fElapsed)
{
    if (m_bHidden) return;
    
	Button::Update(fElapsed);

	if (m_isCheck && m_checkTex != nullptr)
	{
		m_checkTex->Update(fElapsed);
	}
    
    if (m_currTex != nullptr)
    {
        m_currTex->Update(fElapsed);
    }
}
