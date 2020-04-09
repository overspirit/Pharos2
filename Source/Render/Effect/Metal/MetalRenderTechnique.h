#pragma once

namespace Pharos
{
	namespace Render
	{
		class MetalRenderTechnique : public RenderTechnique
		{
		public:
			MetalRenderTechnique();
			virtual ~MetalRenderTechnique(void);

		private:
			MetalRenderer*		m_renderer;

		public:
			virtual bool Create(RenderTechInfo* techInfo);		
			
			virtual RenderTechnique* Clone();
			
			virtual void ApplyToDevice();
		};
	}
}
