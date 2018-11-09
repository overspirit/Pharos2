#pragma once

namespace Pharos
{
	namespace Desktop
	{
		class FontTextureManager : public Utils::Singleton<FontTextureManager>
		{
		public:
			FontTextureManager(void);
			virtual ~FontTextureManager(void);

		private:
			struct CharPos
			{
				uint8 x;
				uint8 y;
			};

			struct FontInfo
			{
				FontInfo()
				{
					font = nullptr;
					currMinValue = 0;
					currMaxValue = 0;

					fontTex = nullptr;
					currCharPosX = 0;
					currCharPosY = 0;
				}

				Font*				font;
				float32				currMinValue;
				float32				currMaxValue;

				RenderTexture*		fontTex;
				uint32				currCharPosX;
				uint32				currCharPosY;

				vector<uint16>		charIndexList;
				vector<CharPos>		charPosList;
			};

		private:
			vector<FontInfo>		m_fontInfoList;

		private:
			void ResumeFontInfo(FontInfo& fontInfo);

			uint32 InsertChar(FontInfo& fontInfo, char16 ch);

		public:
			uint32 AttendFont(Font* font);
			void DestroyAll();

			Vector4Df GetFontCharTexcoord(uint32 fontIndex, char16 ch);
			RenderTexture* GetFontTexture(uint32 fontIndex);
			float32	GetFontDisBase(uint32 fontIndex);
			float32 GetFontDisScale(uint32 fontIndex);
		};
	}
}
