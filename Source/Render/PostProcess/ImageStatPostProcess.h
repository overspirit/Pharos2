#pragma once

namespace Pharos
{
	namespace Render
	{
		class ImageStatPostProcess : public PostProcess
		{
		public:
			ImageStatPostProcess();
			virtual ~ImageStatPostProcess();

		private:
			RenderTechnique*	m_sumLumLogTech;
			RenderTechnique*	m_sumLumIterTech;
			RenderTechnique*	m_adaptedTech;

			RenderTexture*		m_inputTex;

			RenderFrameBuffer*	m_sumLogTarget;
			RenderTexture*		m_sumLogTex;

		public:
			virtual bool Init();

			virtual void SetInputPin(uint32 index, RenderTexture* tex);
			virtual void SetOutputPin(uint32 index, RenderTexture* tex);
			virtual void Apply();
		};
	}
}