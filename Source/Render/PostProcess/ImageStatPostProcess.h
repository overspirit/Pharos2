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
			RenderTexture*		m_outputTex;

			RenderFrameBuffer*	m_sumLogTarget;
			RenderTexture*		m_sumLogTex;

			RenderFrameBuffer*	m_sumIterTarget;
			RenderTexture*		m_sumIterTex;

			RenderFrameBuffer*	m_adaptedLumTarget;
			RenderTexture*		m_adaptedLumTex;

		public:
			virtual bool Init();

			virtual void SetInputPin(uint32 index, RenderTexture* tex);
			virtual void SetOutputPin(uint32 index, RenderTexture* tex);
			virtual void Apply();
		};
	}
}