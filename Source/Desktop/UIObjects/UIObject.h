#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class UIObject
		{
		public:
			UIObject(void);
			virtual ~UIObject(void);
			
		protected:
			string			m_name;	//名字
			string			m_type;	//类型

			UIObject*		m_parent;

			Rect2Di				m_rect;
			Size2Di				m_size;			//矩形大小
			Point2Di			m_center;

			vector<tagAnchor>		m_anchorList;	//锚点列表

			vector<std::pair<IControlViewer*, EVENT_CALLBACK>>	m_callbackList;

		protected:
			const char8* GetAttributeStringValue(XmlNode* node, const char8* name);
			int32 GetAttributeIntValue(XmlNode* node, const char8* name);
			bool GetAttributeBoolValue(XmlNode* node, const char8* name);
			float32 GetAttributeFloatValue(XmlNode* node, const char8* name);
			Color4 GetAttributeColorValue(XmlNode* node, const char8* name);

			Size2Di LoadSize(XmlNode* xmlNode);
			tagAnchor LoadAnchor(XmlNode* xmlNode);

			virtual void DoEvent(const EventArgs& eventArgs);

		public:
			virtual bool LoadFromXml(XmlNode* xmlNode);

			virtual bool onInputMessage(const tagInputMsg& msg);

			virtual void AttachEventCallback(IControlViewer* viewer, EVENT_CALLBACK callback);
			virtual void DettachAllEventCallback();
			
			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);

			virtual const char8* GetName() { return m_name.c_str(); }
			virtual const char8* GetObjectType() { return m_type.c_str(); }
			virtual bool IsObjectType(const char8* type) { return (m_type == type); }

			virtual UIObject* GetParent() { return m_parent; }
			virtual void SetParent(UIObject* parent) { m_parent = parent; }

			virtual const Rect2Di& GetRect() { return m_rect; }
			virtual const Size2Di& GetSize() { return m_size; }
			virtual const Point2Di& GetCenter() { return m_center; }

			virtual void SetSize(int32 width, int32 height) { m_size.width = width; m_size.height = height; }
		};
	}
}
