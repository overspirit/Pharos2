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
			RenderObject*		m_drawColorObj;
			RenderObject*		m_drawTextureObj;
			RenderObject*		m_drawFontObj;

			RenderBlendState*	m_blendState;

			RenderProgram*		m_spriteColorShader;	
			RenderPipeline*		m_spriteColorPipeline;

			RenderProgram*		m_spriteTextureShader;	
			RenderPipeline*		m_spriteTexturePipeline;			

			RenderProgram*		m_distanceFontShader;	
			RenderPipeline*		m_distanceFontPipeline;

			RenderBuffer*		m_vertBuf;
			MemoryBuffer		m_vertMemBuf;
			uint32				m_vertCount;

		public:
			virtual bool Init();
			virtual void Destroy();

			virtual void Render();

			virtual void DrawRect(Color4 color, const Rect2Di& drawRect);
			virtual void DrawWireFrame(Color4 color, const Rect2Di& drawRect);
			
			virtual void DrawColor2D(const DecalColorVertex* vertData, uint32 vertNum);
			virtual void DrawTexture2D(const DecalColorVertex* vertData, uint32 vertNum, RenderResourceSet* resSet);
			virtual void DrawFont2D(const DecalColorVertex* vertData, uint32 vertNum, RenderResourceSet* resSet);
		};
	}
}
