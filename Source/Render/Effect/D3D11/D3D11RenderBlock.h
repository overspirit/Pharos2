#pragma once

namespace Pharos
{
	namespace Render
	{
		class D3D11RenderBlock : public RenderBlock
		{
		public:
			D3D11RenderBlock();
			virtual ~D3D11RenderBlock();

		private:
			D3D11Renderer*			m_renderer;

			D3D11RenderTechnique*	m_tech;
			D3D11RenderLayout*		m_layout;

		public:
			virtual bool Init(RenderLayout* layout, RenderTechnique* tech);

			virtual RenderTechnique* GetTechnique() { return m_tech; }
			virtual RenderLayout* GetLayout() { return m_layout; }

			virtual void ApplyToDevice();
		};
	}
}