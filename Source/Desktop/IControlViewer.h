#pragma once

namespace Pharos
{
	namespace Desktop
	{
		enum EventType
		{
			Click = 0,
			ValueChange,

			Type_End = 0xFF
		};

		struct EventArgs
		{
			EventArgs(EventType type) { this->type = type; }

			EventType type = EventType::Type_End;
			int32 argNum = 0;

			union
			{
				bool			ba;
				int32			na;
				float32			fa;
				const char8*	sa = nullptr;

			} args[1];
		};
		
		class IControlViewer
		{
		public:
			virtual ~IControlViewer(){}

		public:

		};

		typedef bool (IControlViewer::*EVENT_CALLBACK)(UIObject* uiObj, const EventArgs& eventArgs);
	}
}