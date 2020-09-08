#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class DesktopMgr : public Utils::Singleton<DesktopMgr>
		{
		public:
			DesktopMgr();
			virtual ~DesktopMgr();

		private:
			WorldFrame*					m_worldFrame;
			map<string, UIObject*>		m_frameList;
			vector<UIObject*>			m_controlList;

			float32		m_fScaleX;
			float32		m_fScaleY;	

		public:
			template<class T>
			T* GenerateUIObjectCastType(XmlNode* xmlNode, const char8* parentName)
			{
				UIObject* uiObj = this->GenerateUIObject(xmlNode, parentName);
				string type = T::typeWrapper.GetName();
				if (uiObj != nullptr && uiObj->IsObjectType(type.c_str()))
				{
					return static_cast<T*>(uiObj);
				}

				return nullptr;
			}

		public:
			virtual bool Init();
			virtual void Destroy();

			virtual void SetDesktopSize(int32 width, int32 height);
			virtual const Size2Di& GetDesktopSize() { return m_worldFrame->GetSize(); }

			virtual bool LoadUILayoutFile(const char8* szFile);

			virtual bool RegisterControlViewer(const char8* uiName, IControlViewer* viewer, EVENT_CALLBACK callback);

			virtual bool onMouseEvent(const MouseEvent& e);
			virtual bool onKeyboardEvent(const KeyEvent& e);

			virtual void onViewCreate();
			virtual void onViewChangeSize(int32 width, int32 height);
			virtual void onViewDestroy();

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);

			virtual RenderFont* GenerateRenderFont(const char8* fontFilePath);
			virtual RenderImage* GenerateRenderImage();

			virtual WorldFrame* GetWorldFrame() { return m_worldFrame; }
			virtual UIObject* GetControl(const char8* szName);
			virtual UIObject* GenerateUIObject(XmlNode* xmlNode, const char8* parentName);
		};
	}
}
