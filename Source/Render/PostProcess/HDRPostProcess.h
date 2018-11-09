#pragma once

namespace Pharos
{
	namespace Render
	{
		class HDRPostProcess : public PostProcess
		{
		public:
			HDRPostProcess();
			virtual ~HDRPostProcess();

		private:
			PostProcess*		m_imageStatPostProcess;
			PostProcess*		m_lensEffectsPostProcess;
			PostProcess*		m_toneMappingPostProcess;

		public:
			virtual bool Init();

			virtual void SetInputPin(uint32 index, RenderTexture* tex);
			virtual RenderTexture* GetOutputPin(uint32 index);
			virtual uint32 GetOutputPinsNum() { return 1; }

			virtual void Apply();
		};
	}
}