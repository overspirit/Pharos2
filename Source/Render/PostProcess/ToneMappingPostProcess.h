#pragma once

namespace Pharos
{
	namespace Render
	{
		class ToneMappingPostProcess : public PostProcess
		{
		public:
			ToneMappingPostProcess();
			virtual ~ToneMappingPostProcess();

		private:
			RenderTechnique*	m_toneMappingTech;
			//RenderFrameBuffer*	m_toneMappingTarget;
			RenderTexture*		m_toneMappingTex;
			
		public:
			virtual bool Init();

			virtual void Apply();
		};
	}
}
