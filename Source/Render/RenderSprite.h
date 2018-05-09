#pragma once

namespace Pharos
{
	namespace Render
	{
		//画刷，看名字就知道啥意思...
		struct SpriteBrush
		{
			float32 scale;	//缩放系数...
			Color4 color;
			Color4 outlineColor;
			float32 outlineScale;
			Color4 shadowColor;
			int32 shadowOffsetX;
			int32 shadowOffsetY;
		};

		class RenderSprite : public Utils::Singleton<RenderSprite>
		{
		public:
			RenderSprite(void);
			virtual ~RenderSprite(void);

		private:
			struct Vertex
			{
				Vector3Df	pos;
				Color4		color;
				Vector2Df	tex;
			};

			//存储字体中每个字存储的纹理索引和纹理位置
			struct CharInfo
			{
				char16 ch;
				uint8	index;	//纹理信息索引，在m_submitInfoList中
				float32	left;	//在纹理中的位置信息，下同
				float32 top;
				float32 right;
				float32 bottom;
			};

			//字体信息，存储字体中每个字的信息
			struct FontInfo
			{
				FontInfo() { memset(charIndex, 0xFF, sizeof(charIndex)); }
				Font* font;
				uint16 charIndex[65535];	//每个字信息的索引，索引charInfoList
				vector<CharInfo> charInfoList;	//每个字信息，使用之后才添加一个信息
			};

			//字体纹理，以及每个字体纹理使用的顶点数据，shader
			struct SubmitInfo
			{
				MemoryBuffer		vertData;
				uint32				dataVertNum;

				RenderTexture*		fontTex;

				//普通字体的shader
				RenderTechnique*	fontTech;
				RenderVariable*		fontTechBaseVar;
				RenderVariable*		fontTechScaleVar;
				RenderVariable*		fontTechColorVar;

				//字体阴影的shader
				RenderTechnique*	fontShadowTech;
				RenderVariable*	fontShadowTechBaseVar;
				RenderVariable*	fontShadowTechScaleVar;
				RenderVariable*	fontShadowTechColorVar;
				RenderVariable*	fontShadowTechShadowOffsetVar;

				//带描边的字体shader
				RenderTechnique*	fontOutlineTech;
				RenderVariable*	fontOutlineTechBaseVar;
				RenderVariable*	fontOutlineTechScaleVar;				
				RenderVariable*	fontOutlineTechColorVar;
				RenderVariable*	fontOutlineTechOutlineScaleVar;
				RenderVariable*	fontOutlineTechOutlineColorVar;

				//IRenderBlockPtr		fontTechBlock;
				//IRenderBlockPtr		fontOutlineTechBlock;
				//IRenderBlockPtr		fontShadowTechBlock;
			};

			//递交的绘制信息，
			struct SubmitBlock
			{
				MemoryBuffer		vertData;
				uint32				dataVertNum;

				//IRenderBlockPtr	fontTechBlock;
				//IRenderBlockPtr fontShadowTechBlock;
                
                RenderTechnique*	fontTech;
                RenderTechnique*	fontShadowTech;
			};

		private:
			Renderer*		m_renderer;
			RenderLayout*	m_layout;
			uint32				m_layoutStart;

			//为了保证每个字体只用一个FontInfo，使用了map，RenderFont保存的是m_fontInfoList中的索引
			map<Font*, uint32>		m_fontInfoIndexMap;
			vector<FontInfo>			m_fontInfoList;
			
			//每使用一张字体纹理，就添加一个SubmitInfo，
			vector<SubmitInfo>			m_submitInfoList;
			Point2Di					m_currFontTexPos;		//当前使用的字体纹理中使用的位置

			//这个索引对应CharInfo中的uint8的index,每次提交绘制信息时都检查一下有没有对应使用该
			//索引的SubmitBlock，下次使用SubmitBlock中的信息直接绘制
			uint16					m_submitBlockIndex[255];	
			vector<SubmitBlock>		m_submitBlockList;

			MemoryBuffer			m_imageVertDataBuf;

			RenderTechnique*		m_renderColorTech;

			RenderBlock*			m_renderBlock;

		private:
			void InsertChar(FontInfo& fontInfo, char16 ch, uint16 charWidth, uint16 charHeight);

			void AddSumitInfo();

		public:
			bool Init(Renderer* renderer);
			void Destroy();			

			uint32 GetFontInfoIndex(Font* font);

			//可以在一次绘制时多次使用RenderText，最后调用PostSubmitBlock提交绘制
			void RenderText(uint32 fontIndex, const char8* text, int32 textLen, const SpriteBrush& brush, int32 x, int32 y);
			void RenderRect(RenderTechnique* renderTech, const Vector4Df& uv, Color4 color, const Rect2Di& destRect);
			void RenderBox(const Rect2Di& box, Color4 color);

			void PostSubmitBlock();
            
			void Resume();
		};
	}
}
