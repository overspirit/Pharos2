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

		virtual bool onMouseEvent(const MouseEvent& event) = 0;
		virtual bool onKeyboardEvent(const KeyEvent& event) = 0;
        virtual void onWindowChangeSize(int32 width, int32 height) = 0;
        
		virtual void Update(float32 fElapsed) = 0;
	};
}
