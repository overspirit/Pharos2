#pragma once

namespace Pharos
{
	namespace Core
	{
		struct tagCharInfo
		{
			//字符的绘制宽度和高度，也就是下一个字符应该起始的位置，
			//本字符的x位置加上adv_x就是下一个字符的x位置, y值同理
			uint16 adv_x;
			uint16 adv_y;

			int16 off_x;	//字符的x偏移，决定字符的绘制起始x位置
			int16 off_y;	//字符的y偏移，决定字符的绘制起始y位置

			uint16 width;	//字符的宽度
			uint16 height;	//字符的高度
		};
				
		class GlyphFilter;

		//Font类
		//使用FreeType库读取字符信息，将其转换距离场信息输出。
		//计算距离场的代码来源于KlayGE,代码的大体思想是先用FT库读取一个128x128的点阵图信息，
		//然后降采样到64x64, 再梯度采样，最后降采样到32x32，计算出距离场。
		//我对原先的代码最大的改动是:原先的代码因为是计算所有有效的字符，所以能知道字符梯度场
		//中的最小值和最大值，然后将最小值映射成0,最大值映射成255，存储在一个8位纹理信息中，
		//而我是读取一个字符就映射，所以不知道最小值和最大值，所以我才用了最小值和最大值固定的
		//的映射方法，这样做必然会损失一些精度。所以取一个比较合适的最小值和最大值很重要。
		//字符的绘制方法就是取字符的距离场，然后剔除到指定距离的像素就成为了要绘制的字符，
		//剔除的方法就是将采样的纹理值 value * scale + base
		//scale和base跟字符大小，最大值，最小值有关系,值我是根据KlayGE中的代码推导得来
		//scale 为 min_value * char_size + 1.0
		//base 为 (max_value - min_value) * char_size
		class Font : public ResBase
		{
		private:
			Font(FT_Library fontLib);
			virtual ~Font();

			friend class ResourceManager;

		private:
			struct tagCharInfoInternal : public tagCharInfo
			{
				tagCharInfoInternal()
				{
					adv_x = 0;
					adv_y = 0;

					off_x = 0;
					off_y = 0;

					width = 0;
					height = 0;
				}

				vector<float32> char_dis_data;
			};

		private:
			FT_Library		m_fontLib;
			FT_Face			m_face;

			MemoryBuffer	m_fontData;

			vector<uint16>					m_char_index;	//存储每个字符在的m_char_infos的索引，初始化为0xFFFF
			vector<tagCharInfoInternal>		m_char_infos;	//每读取一个字符，就增加一个CharInfo

			GlyphFilter*		m_filter;
			uint32				m_filterSize;
			uint32				m_samplerSize;
			uint32				m_charPixelSize;
			vector<uint8_t>		m_charBitmapBuffer;		//字符的原点阵信息

			//用于映射到字符纹理中的最小值
			//距离场计算得到的是浮点数，必须映射到8位纹理中，
			//我暂时取m_min_value为-0.775，m_max_value为0.5
			float32		m_min_value;
			float32		m_max_value;

		private:
			void LoadDisChar(char16 ch);

		public:
			virtual bool Open(const char8* path);
			virtual bool Save(const char8* path);

			virtual float32* GetDisCharData(char16 ch);
			virtual const tagCharInfo& GetDisCharInfo(char16 ch);
			virtual uint32 GetDisCharSize() { return m_filterSize; }
			//virtual float32 GetDisBase() { return m_min_value * m_filterSize + 1.0f; }
			//virtual float32 GetDisScale() { return (m_max_value - m_min_value) * m_filterSize; }
			virtual float32 GetDisMinValue() { return m_min_value; }
			virtual float32 GetDisMaxValue() { return m_max_value; }
			virtual bool SaveCharBitmap(const char8* path);
		};

		class GlyphFilter
		{
		public:
			GlyphFilter();
			~GlyphFilter();

		private:
			uint32		m_char_size;	//距离场的字符大小，固定32

			float32		m_char_scale;	//这个不清楚，定值根据m_char_size计算而来
			float32		m_filterMaxValue;
			float32		m_filterMinValue;

			vector<float32>			m_aa_char_bitmap_2x;	//降采样1次的信息
			vector<float32>			m_aa_char_bitmap;		//最后采样的信息
			vector<Vector2Df>		m_grad;
			vector<float32>			m_outside;
			vector<float32>			m_inside;
			vector<float32>			m_filter;

		private:
			float32 EdgeDistance(const Vector2Df& grad, float32 val);
			float32 AADist(const vector<float32>& img, const vector<Vector2Df>& grad, int32 width, int32 offset_addr, const Vector2Df& offset_dist_xy, const Vector2Df& new_dist);
			bool UpdateDistance(int32 x, int32 y, int32 dx, int32 dy, const vector<float32>& img, int32 width, const vector<Vector2Df>& grad, vector<Vector2Df>& dist_xy, vector<float32>& dist);
			void AAEuclideanDistance(const vector<float32>& img, const vector<Vector2Df>& grad, int32 width, int32 height, vector<float32>& dist);
			void ComputeGradient(const vector<float32>& img_2x, int32 w, int32 h, vector<Vector2Df>& grad);

		public:
			void SetFilterSize(uint32 charSize);
			float32* FilterCharacterGlyph(uint8* charBitmapData, uint32 bitmapSize);
			bool SaveFilterImage(const char8* path);
			float GetFilterMaxValue() { return m_filterMaxValue; }
			float GetFilterMinValue() { return m_filterMinValue; }
		};
	}
}
