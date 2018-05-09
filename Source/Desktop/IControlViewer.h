#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class IControlViewer
		{
		public:
			virtual ~IControlViewer(){}

		public:
			virtual void onControlCreate(const char8* name, int32 v1, float32 v2) = 0;

			virtual void onControlValueChange(const char8* name, int32 v1, float32 v2) = 0;
		};
	}
}