#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "DesktopGlobal.h"

Progress::Progress(void)
{
	m_type = "Progress";

	m_putType = EPT_End;

	m_minValue = 0;
	m_maxValue = 100.0f;
	m_value = 0;
}

Progress::~Progress(void)
{

}

bool Progress::LoadFromXml(XmlNode* xmlNode)
{
 	if(!Frame::LoadFromXml(xmlNode)) return false;
 	
 	m_putType = GetPutType(GetAttributeStringValue(xmlNode, "puttype"));
 	m_minValue = GetAttributeFloatValue(xmlNode, "min");
 	m_maxValue = GetAttributeFloatValue(xmlNode, "max");
 	m_value = GetAttributeFloatValue(xmlNode, "value");
	
	m_progressTex = GetTextureFromXmlNode(xmlNode, "Bar");
	m_progressEdge = GetEdgeFromXmlNode(xmlNode, "Bar");

 	if(m_putType == EPT_End) m_putType = EPT_Horizontal;
 
 	m_value = Math::clamp(m_value, m_minValue, m_maxValue);

	return true;
}

void Progress::Update(float32 fElapsed)
{
	Frame::Update(fElapsed);

	//计算进度条比例位置
	float32 scale = (m_value - m_minValue) / (m_maxValue - m_minValue);	

	m_progressRect.left = m_rect.left + m_progressEdge.left;
	m_progressRect.bottom = m_rect.bottom - m_progressEdge.bottom;

	//根据摆放类型计算进度条的矩形
	switch(m_putType)
	{
	case EPT_Horizontal:
		{	
			int32 barWidth = (m_rect.right - m_progressEdge.right) - m_progressRect.left;
			m_progressRect.right = m_progressRect.left + (int32)(barWidth * scale);
			m_progressRect.top = m_rect.top + m_progressEdge.top;
		}
		break;
	case EPT_Vertical:
		{
			int32 barHeight = m_progressRect.bottom - (m_rect.top + m_progressEdge.top);
			m_progressRect.top = m_progressRect.bottom - (int32)(barHeight * scale);
			m_progressRect.right = m_rect.right - m_progressEdge.right;
		}
		break;
	}

	if (m_progressTex != nullptr)
	{
		m_progressTex->SetDrawRect(m_progressRect);
	}
}

void Progress::Render(float32 fElapsed)
{
	Frame::Render(fElapsed);

	if (m_progressTex != nullptr)
	{
		m_progressTex->Render(fElapsed);
	}
}

void Progress::SetValue(float32 fValue)
{
	m_value = fValue;

	//保证值在最大值和最小值之间
	m_value = Math::clamp(m_value, m_minValue, m_maxValue);
}