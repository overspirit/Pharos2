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
			//vector<D3D11ConstantBuffer*>	m_constantBufList;

			D3D11Renderer*		m_renderer;

		public:
			virtual bool Create(const char8* effectText, const TechniqueInfo& info);			
			
			virtual RenderTechnique* Clone();

			virtual RenderVariable* GenerateVariable(const char8* name, uint32 dataSize);

			virtual void ApplyToDevice();
		};
	}
}