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
			RenderFrameBuffer*	m_sumLogTarget;
			RenderTexture*		m_sumLogTex;

			RenderTechnique*	m_sumLumIterTech16;
			RenderFrameBuffer*	m_sumIterTarget16;
			RenderTexture*		m_sumIterTex16;

			RenderTechnique*	m_sumLumIterTech4;
			RenderFrameBuffer*	m_sumIterTarget4;
			RenderTexture*		m_sumIterTex4;

			RenderTechnique*	m_adaptedTech;
			RenderFrameBuffer*	m_adaptedLumTarget;
			RenderTexture*		m_adaptedLumTex;
			
		public:
			virtual bool Init();

			virtual void Apply();
		};
	}
}