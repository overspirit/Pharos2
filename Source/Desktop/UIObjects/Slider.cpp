#include "PreCompile.h"
#include "Pharos.h"

Slider::Slider()
{
	m_type = "Slider";

	m_putType = EPT_Horizontal;

	m_minValue = 0;
	m_maxValue = 100.0f;
	m_step = 0;
	m_currValue = 0;
	m_wheelSpeed = 1.0f;

	m_bInThumb = false;
	m_thumbState = EBS_PopUp;

	m_thumbBackTex = nullptr;
	m_thumbPushTex = nullptr;
	m_thumbHighlightTex = nullptr;
	m_thumbDisableTex = nullptr;

	m_currThumbTex = nullptr;
}

Slider::~Slider()
{

}

bool Slider::LoadFromXml(XmlNode* xmlNode)
{
	if (!Frame::LoadFromXml(xmlNode)) return false;

	m_putType = GetPutType(GetAttributeStringValue(xmlNode, "put_type"));
	m_minValue = GetAttributeFloatValue(xmlNode, "min");
	m_maxValue = GetAttributeFloatValue(xmlNode, "max");
	m_step = GetAttributeFloatValue(xmlNode, "step");
	m_currValue = GetAttributeFloatValue(xmlNode, "value");
	m_wheelSpeed = GetAttributeFloatValue(xmlNode, "wheel_speed");

	XmlNode* thumbNode = xmlNode->GetFirstNode("Thumb");
	if (thumbNode != nullptr)
	{
		m_thumbBackTex = GetTextureFromXmlNode(thumbNode, "BackStyle");
		m_thumbPushTex = GetTextureFromXmlNode(thumbNode, "PushStyle");
		m_thumbHighlightTex = GetTextureFromXmlNode(thumbNode, "HighlightStyle");
		m_thumbDisableTex = GetTextureFromXmlNode(thumbNode, "DisableStyle");

		XmlNode* thumbSizeNode = thumbNode->GetFirstNode("Size");
		if (thumbSizeNode != nullptr)
		{
			m_thumbSize = LoadSize(thumbSizeNode);
		}
	}

	m_thumbEdge = GetEdgeFromXmlNode(xmlNode, "Thumb");

	m_currThumbTex = nullptr;

	if (m_putType == EPT_End) m_putType = EPT_Horizontal;
	m_currValue = Math::clamp(m_currValue, m_minValue, m_maxValue);

	if (m_step != 0)
	{
		float32 range = m_maxValue - m_minValue;
		m_step = range / ceilf(range / m_step);
		m_currValue = (int32)(m_currValue / m_step) * m_step;
	}

	return true;
}

void Slider::SetValueRange(float32 minValue, float32 maxValue, float32 step)
{
	m_minValue = minValue;
	m_maxValue = maxValue;
	m_step = step;
	m_currValue = minValue;

	EventArgs eventArgs(EventType::ValueChange);
	eventArgs.argNum = 1;
	eventArgs.args[0].fa = m_currValue;
	DoEvent(eventArgs);
}

void Slider::SetValue(float32 value)
{
	m_currValue = value;

	EventArgs eventArgs(EventType::ValueChange);
	eventArgs.argNum = 1;
	eventArgs.args[0].fa = m_currValue;
	DoEvent(eventArgs);
}

void Slider::Update(float32 fElapsed)
{
	Frame::Update(fElapsed);

	int32 thumbCenterX = 0, thumbCenterY = 0;

	//计算滑块中心点的位置(比例点)
	//(m_fValue - m_fMinValue) / (m_fMaxValue - m_fMinValue)为值所在最大值和最小值之间的比例
	float32 scale = (m_currValue - m_minValue) / (m_maxValue - m_minValue);
	int32 edgeLeft = m_rect.left + m_thumbEdge.left;
	int32 edgeTop = m_rect.top + m_thumbEdge.top;
	int32 edgeWidth = (m_rect.right - m_thumbEdge.right) - edgeLeft;
	int32 edgeHeight = (m_rect.bottom - m_thumbEdge.bottom) - edgeTop;

	//根据摆放类型，计算位置
	//pos乘以宽或者高就是值所在的位置
	switch (m_putType)
	{
	case EPT_Horizontal:
	{
		thumbCenterX = edgeLeft + (int32)(edgeWidth * scale);
		thumbCenterY = edgeTop + edgeHeight / 2;
	}
	break;
	case EPT_Vertical:
	{
		thumbCenterX = edgeLeft + edgeWidth / 2;
		thumbCenterY = edgeTop + (int32)(edgeHeight * scale);
	}
	break;
	}

	//计算滑块矩形
	int32 rtWidth = m_thumbSize.width / 2;
	int32 rtHeight = m_thumbSize.height / 2;
	m_rtThumb.left = thumbCenterX - rtWidth;
	m_rtThumb.right = thumbCenterX + rtWidth;
	m_rtThumb.top = thumbCenterY - rtHeight;
	m_rtThumb.bottom = thumbCenterY + rtHeight;

	if (m_thumbBackTex != nullptr)
	{
		m_thumbBackTex->SetDrawRect(m_rtThumb);
	}

	if (m_currThumbTex != nullptr)
	{
		m_currThumbTex->SetDrawRect(m_rtThumb);
	}
}

void Slider::Render(float32 fElapsed)
{
	if (m_bHidden) return;

	Frame::Render(fElapsed);

	if (m_thumbBackTex != nullptr)
	{
		m_thumbBackTex->Render(fElapsed);
	}

	if (m_currThumbTex != nullptr)
	{
		m_currThumbTex->Render(fElapsed);
	}
}

void Slider::SetThumbState(ButtonState state)
{
	m_thumbState = state;

	switch (state)
	{
	case EBS_PopUp:
	{
		m_currThumbTex = nullptr;
	}
	break;
	case EBS_PushDown:
	{
		m_currThumbTex = m_thumbPushTex;
	}
	break;
	case EBS_Highlight:
	{
		m_currThumbTex = m_thumbHighlightTex;
	}
	break;
	case EBS_Disabled:
	{
		m_currThumbTex = m_thumbDisableTex;
	}
	break;
	default:
	{
		m_currThumbTex = nullptr;
	}
	}
}

bool Slider::onLeftButtonDown(const tagInputMsg& msg)
{
	if (!m_bEnable) return false;

	//if (m_rect.IsPointInside(msg.p1, msg.p2))
	{
		if (m_rtThumb.IsPointInside(msg.p1, msg.p2))
		{
			m_bInThumb = true;

			SetThumbState(EBS_PushDown);

			return true;
		}
	}
	return m_bInThumb;
}

bool Slider::onLeftButtonUp(const tagInputMsg& msg)
{
	if (!m_bEnable) return false;	

	if (m_rtThumb.IsPointInside(msg.p1, msg.p2))
	{
		SetThumbState(EBS_Highlight);
	}
	else
	{
		SetThumbState(EBS_PopUp);
	}

	m_bInThumb = false;

	return false;
}

bool Slider::onMouseMove(const tagInputMsg& msg)
{
	if (!m_bEnable) return false;

	if (m_bInThumb)
	{
		SetThumbState(EBS_PushDown);
	}
	else
	{
		if (m_rtThumb.IsPointInside(msg.p1, msg.p2))
		{
			SetThumbState(EBS_Highlight);
		}
		else
		{
			SetThumbState(EBS_PopUp);
		}
	}

	if (m_bInThumb)
	{
		//根据摆放类型取出参与计算的值
		int32 start = 0, mouse = 0, len = 0;
		switch (m_putType)
		{
		case EPT_Horizontal:
		{
			mouse = msg.p1;
			start = m_rect.left;
			len = m_rect.right - start;
		}
		break;
		case EPT_Vertical:
		{
			mouse = msg.p2;
			start = m_rect.top;
			len = m_rect.bottom - start;
		}
		break;
		}

		//如果步进为0，则是平滑移动
		if (m_step == 0.0f)
		{
			m_currValue = (float32)(mouse - start) / len * (m_maxValue - m_minValue) + m_minValue;
		}
		else
		{
			float32 stepLen = len * m_step / (m_maxValue - m_minValue);
			m_currValue = (float32)Math::round((mouse - start) / stepLen) * m_step + m_minValue;
		}

		//保证值在最大值和最小值之间
		m_currValue = Math::clamp(m_currValue, m_minValue, m_maxValue);

		EventArgs eventArgs(EventType::ValueChange);
		eventArgs.argNum = 1;
		eventArgs.args[0].fa = m_currValue;
		DoEvent(eventArgs);

		return true;
	}

	return m_bInThumb;
}
