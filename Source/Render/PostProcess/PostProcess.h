#pragma once

namespace Pharos
{
	namespace Render
	{
		class PostProcess
		{
		public:
			PostProcess();
			virtual ~PostProcess();

		public:
			virtual bool Init();

			virtual void SetInputPin(uint32 index, RenderTexture* tex) = 0;
			virtual void SetOutputPin(uint32 index, RenderTexture* tex) = 0;
			virtual void Apply() = 0;
		};
	}
}