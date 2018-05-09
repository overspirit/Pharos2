#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"

CheckButton::CheckButton(void)
{
	m_type = "CheckButton";

	m_isCheck = false;
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

void CheckButton::PushEvent(int32 v1, float32 v2)
{
	m_isCheck = !m_isCheck;

	sDesktopMgr->ReceivedEvent(m_name.c_str(), m_isCheck, v2);
}

void CheckButton::Update(float32 fElapsed)
{
	Button::Update(fElapsed);

	if (m_isCheck && m_checkTex != nullptr)
	{
		m_checkTex->Update(fElapsed);
	}
}

void CheckButton::Render(float32 fElapsed)
{
	Frame::Render(fElapsed);

	if (m_isCheck && m_checkTex != nullptr)
	{
		m_checkTex->Render(fElapsed);
	}

	if (m_currTex != nullptr)
	{
		m_currTex->Render(fElapsed);
	}
}