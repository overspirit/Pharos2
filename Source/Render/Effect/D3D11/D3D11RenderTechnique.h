#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11RenderTechnique : public RenderTechnique
		{
		public:
			D3D11RenderTechnique();
			virtual ~D3D11RenderTechnique(void);

		private:
			D3D11Renderer*		m_renderer;

		public:
			virtual bool Create(const char8* effectText, const TechniqueInfo& info);			
			
			virtual RenderTechnique* Clone();
			
			virtual void ApplyToDevice();
		};
	}
}