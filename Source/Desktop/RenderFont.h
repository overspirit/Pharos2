#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class RenderFont
		{
		public:
			RenderFont(void);
			virtual ~RenderFont(void);

		private:
			Font*				m_font;			
			uint32				m_fontInfoIndex;
			RenderTexture*		m_fontTex;
			
			Size2Di		m_charSize;
			Color4		m_charColor;
			float32		m_charOutlineScale;			
			Color4		m_charOutlineColor;
			Color4		m_charShadowColor;
			Size2Di		m_charShadowOffset;

//			RenderTechnique*	m_fontTech;
//			RenderVariable*		m_fontTextureVar;
//			RenderVariable*		m_fontDistanceVar;
//			RenderVariable*		m_fontOffsetVar;
//			RenderVariable*		m_fontColorVar;
//			RenderVariable*		m_outlineScaleVar;			
//			RenderVariable*		m_outlineColorVar;
//
//			RenderTechnique*	m_fontShadowTech;
//			RenderVariable*		m_fontShadowTextureVar;
//			RenderVariable*		m_fontShadowDistanceVar;
//			RenderVariable*		m_fontShadowOffsetVar;
//			RenderVariable*		m_fontShadowColorVar;
//			RenderVariable*		m_shadowOutlineScaleVar;			
//			RenderVariable*		m_shadowOutlineColorVar;

			vector<DecalColorVertex>		m_vertBuffer;

		private:
			uint32 FillTextVertBuffer(const char8* text, int32 textLen, int32 x, int32 y);

		public:
			bool LoadFont(const char8* fontFilePath);

			void RenderText(const char8* text, int32 textLen, int32 x, int32 y);

			void SetFontCharSize(int32 width, int32 height);
			void SetFontCharColor(Color4 charColor);
			void SetOutlineScale(float32 charOutlineScale);
			void SetOutlineColor(Color4 charOuterColor);			
			void SetShadowColor(Color4 charShadowColor);
			void SetShadowOffset(int32 offsetX, int32 offsetY);
		};
	}
}
