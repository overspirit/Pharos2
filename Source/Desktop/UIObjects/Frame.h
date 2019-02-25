#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class Frame : public UIObject
		{
		public:
			Frame(void);
			virtual ~Frame(void);

			DECLARE_UI_CLASS(Frame);

		protected:
			Texture*		m_backDropTex;
			Rect2Di			m_backDropEdge;

			list<UIObject*>		m_childList;

			bool	m_bEnable;
			bool	m_bDrawGray;		//绘制灰色的禁用纹理
			bool	m_bHidden;			

		protected:
			Texture* GetTextureFromXmlNode(XmlNode* xmlNode, const char8* name);
			Rect2Di GetEdgeFromXmlNode(XmlNode* xmlNode, const char8* name);

			virtual bool onLeftButtonDown(const tagInputMsg& msg);
			virtual bool onLeftButtonUp(const tagInputMsg& msg);
			virtual bool onMouseMove(const tagInputMsg& msg);	

		public:
			virtual bool LoadFromXml(XmlNode* xmlNode);

			virtual bool onInputMessage(const tagInputMsg& msg);			

			virtual void AddChild(UIObject* child);

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);

			virtual void SetEnable(bool enable) { m_bEnable = enable; }
			virtual void SetHidden(bool hidden) { m_bHidden = hidden; }
		};
	}
}
