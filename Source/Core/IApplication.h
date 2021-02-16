#pragma once

namespace Pharos
{
	class IApplication
	{
	public:
		virtual ~IApplication(){}

	public:
		virtual bool Init() = 0;
		virtual void Destroy() = 0;

		virtual bool onMouseEvent(const MouseEvent& event) { return false; }
		virtual bool onKeyboardEvent(const KeyEvent& event) { return false; }
        virtual void onWindowChangeSize(int32 width, int32 height) { }
        
		virtual void onPreSceneUpdate(float32 fElapsed) { }
		virtual void onPostSceneUpdate(float32 fElapsed) { }
        virtual void onPreRender(float32 fElapsed) { }
        virtual void onPostRender(float32 fElapsed) { }
	};
}
