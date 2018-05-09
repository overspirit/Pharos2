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
			TexturePtr		m_backDropTex;
			Rect2Di			m_backDropEdge;

			//防止循环引用，所以用weak_ptr
			list<std::weak_ptr<UIObject>>	m_childList;

			bool	m_bEnable;
			bool	m_bDrawGray;		//绘制灰色的禁用纹理
			bool	m_bHidden;

		protected:
			TexturePtr GetTextureFromXmlNode(XmlNode* xmlNode, const char8* name);
			Rect2Di GetEdgeFromXmlNode(XmlNode* xmlNode, const char8* name);

			virtual bool onLeftButtonDown(const tagInputMsg& msg);
			virtual bool onLeftButtonUp(const tagInputMsg& msg);
			virtual bool onMouseMove(const tagInputMsg& msg);

			virtual void PushEvent(int32 v1, float32 v2);

		public:
			virtual bool LoadFromXml(XmlNode* xmlNode);

			virtual bool onInputMessage(const tagInputMsg& msg);

			virtual void AddChild(UIObjectPtr child);

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);
		};
	}

	typedef std::shared_ptr<Desktop::Frame>	FramePtr;
}
