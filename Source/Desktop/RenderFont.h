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
			struct FontDrawInfo
			{
				Vector2Df fontDistance;
				Vector2Df fontOffset;
				Color4f charColor;
				Color4f outlineColor;
				float outlineScale;
			};

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

			FontDrawInfo	m_drawInfo;
			RenderBuffer*	m_fontBuffer;

			RenderResourceSet*		m_resSet;

			vector<DecalColorVertex>		m_vertBuffer;
			//uint32							m_vertNum;

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
