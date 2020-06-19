#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class RenderSprite : public Utils::Singleton<RenderSprite>
		{
		public:
			RenderSprite(void);
			virtual ~RenderSprite(void);

		private:
			//RenderTechnique*		m_renderTech;

		public:
			virtual void RenderRect(Color4 color, const Rect2Di& drawRect);
			virtual void RenderWireFrame(Color4 color, const Rect2Di& drawRect);
		};
	}
}
