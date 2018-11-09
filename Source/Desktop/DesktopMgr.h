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
			WorldFramePtr					m_worldFrame;
			map<string, UIObjectPtr>		m_frameList;
			vector<UIObjectPtr>				m_controlList;

			float32		m_fScaleX;
			float32		m_fScaleY;

			vector<IControlViewer*>		m_viewerList;
			
			RenderBlock*		m_renderBlock;
			RenderLayout*		m_renderLayout;
			MemoryBuffer		m_layoutBuffer;
			uint32				m_vertCount;

		public:
			template<class T>
			std::shared_ptr<T> GenerateUIObjectCastType(XmlNode* xmlNode, const char8* parentName)
			{
				UIObjectPtr uiObj = this->GenerateUIObject(xmlNode, parentName);
				string type = T::typeWrapper.GetName();
				if (uiObj != nullptr && uiObj->IsObjectType(type.c_str()))
				{
					return static_pointer_cast<T>(uiObj);
				}

				return nullptr;
			}

		public:
			virtual bool Init();
			virtual void Destroy();

			virtual void SetDesktopSize(int32 width, int32 height);
			virtual const Size2Di& GetDesktopSize(){ return m_worldFrame->GetSize(); }

			virtual bool LoadUILayoutFile(const char8* szFile);
			
			virtual bool RegisterControlViewer(IControlViewer* viewer);
			virtual void ReceivedEvent(const char8* name, int32 v1, float32 v2);

			virtual bool onMouseEvent(const MouseEvent& e);
			virtual bool onKeyboardEvent(const KeyEvent& e);

			virtual void onViewCreate();
			virtual void onViewChangeSize(int32 width, int32 height);
			virtual void onViewDestroy();

			virtual void Update(float32 fElapsed);
			virtual void Render(float32 fElapsed);

			virtual void PushRenderPatch(const DecalColorVertex* vertData, uint32 vertNum, RenderTechnique* tech, DrawType drawType = EDT_TRIANGLELIST);

			virtual RenderFont* GenerateRenderFont(const char8* fontFilePath);
			virtual RenderImage* GenerateRenderImage(const char8* imageFilePath);

			virtual WorldFramePtr GetWorldFrame() { return m_worldFrame; }
			virtual UIObjectPtr GetControl(const char8* szName);
			virtual UIObjectPtr GenerateUIObject(XmlNode* xmlNode, const char8* parentName);
		};
	}
}