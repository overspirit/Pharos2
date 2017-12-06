#pragma once

namespace Pharos
{
	namespace Render
	{
		class IRenderCallback
		{
		public:
			virtual ~IRenderCallback(){}

		public:
			virtual void onRender(float32 elapsed) = 0;
		};
	}
}