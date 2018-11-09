#pragma once

namespace Pharos
{
	namespace Render
	{
		class LensEffectsPostProcess : public PostProcess
		{
		public:
			LensEffectsPostProcess();
			virtual ~LensEffectsPostProcess();

		public:
			virtual bool Init();

			virtual void Apply();
		};
	}
}