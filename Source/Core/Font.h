#pragma once

namespace Pharos
{
	namespace Core
	{
		struct tagCharInfo
		{
			tagCharInfo() { ZeroMemory(this, sizeof(tagCharInfo)); }

			//字符的绘制宽度，也就是下一个字符应该起始的位置，
			//本字符的x位置加上adv_x就是下一个字符的x位置
			uint16 adv_x;

			//字符的绘制高度，也就是下一个字符应该起始的位置，
			//本字符的y位置加上adv_y就是下一个字符的y位置，一般为0
			uint16 adv_y;

			int16 left;	//字符的x偏移，决定字符的绘制起始x位置
			int16 top;	//字符的y偏移，决定字符的绘制起始y位置

			uint16 width;	//字符的宽度
			uint16 height;	//字符的高度

							//用于索引字符信息
			uint32 dis_data_index;
		};

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
		public:
			Font(FT_Library fontLib);
			virtual ~Font();

		private:
			struct raster_user_struct
			{
				FT_BBox bbox;
				int32 buf_width;
				int32 buf_height;
				uint32 internal_char_size;
				bool non_empty;
				uint8* char_bitmap;
			};

		private:
			FT_Library		m_fontLib;
			FT_Face			m_face;

			MemoryBuffer	m_fontData;
			
			vector<uint16>	m_char_index;	//存储每个字符在的m_char_infos的索引，初始化为0xFFFF
			vector<tagCharInfo>	m_char_infos;	//每读取一个字符，就增加一个CharInfo
			vector<uint8>	m_char_dis_data;	//存储所有的字符信息，用CharInfo中的dis_data_index索引这里面的数据

			uint32		m_internal_char_size;	//预读取的字符大小，固定值128
			uint32		m_char_size;	//距离场的字符大小，固定32
			float32		m_char_scale;	//这个不清楚，定值根据m_char_size计算而来

			raster_user_struct		m_raster_user;
			FT_Raster_Params		m_params;

			//下列成员都是为了防止反复分配内存造成的瓶颈而预先分配的成员
			//每次读取一个字符时，都要将下列信息初始化一遍
			//////////////////////////////////////////////////////////////////////////
			vector<uint8_t>	m_char_bitmap;	//字符的原点阵信息
			vector<float32>	m_aa_char_bitmap_2x;	//降采样1次的信息
			vector<float32>	m_aa_char_bitmap;		//最后采样的信息
			vector<Vector2Df>	m_grad;		//不清楚
			vector<float32>	m_outside;	//不清楚
			vector<float32>	m_inside;	//不清楚
			//////////////////////////////////////////////////////////////////////////

			//用于映射到字符纹理中的最小值
			//距离场计算得到的是浮点数，必须映射到8位纹理中，
			//我暂时取m_min_value为-0.775，m_max_value为0.5
			float32		m_min_value;
			float32		m_max_value;

		private:
			float32 EdgeDistance(const Vector2Df& grad, float32 val);

			float32 AADist(const vector<float32>& img, const vector<Vector2Df>& grad, 
				int32 width, int32 offset_addr, const Vector2Df& offset_dist_xy, const Vector2Df& new_dist);

			bool UpdateDistance(int32 x, int32 y, int32 dx, int32 dy, const vector<float32>& img, 
				int32 width, const vector<Vector2Df>& grad, vector<Vector2Df>& dist_xy, vector<float32>& dist);

			void AAEuclideanDistance(const vector<float32>& img, const vector<Vector2Df>& grad, int32 width, int32 height, vector<float32>& dist);

			void ComputeGradient(const vector<float32>& img_2x, int32 w, int32 h, vector<Vector2Df>& grad);

			static void RasterCallback(int32 y, int32 count, FT_Span const * const spans, void* const user);

			void LoadDisChar(char16 ch);

		public:
			virtual bool Open(const char8* path);
			virtual bool Save(const char8* path);

			virtual uint8* GetDisCharData(char16 ch);
			virtual const tagCharInfo& GetDisCharInfo(char16 ch);
			virtual uint32 GetDisCharSize(){ return m_char_size; }
			virtual float32 GetDisBase(){ return m_min_value * m_char_size + 1.0f; }
			virtual float32 GetDisScale(){ return (m_max_value - m_min_value) * m_char_size; }
		};
	}
}
