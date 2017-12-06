#include "PreCompile.h"
#include "CoreGlobal.h"

Font::Font(FT_Library fontLib)
{
	m_strResType = "FONT";

	m_fontLib = fontLib;

	m_face = nullptr;
}

Font::~Font()
{
	if (m_face != nullptr)
	{
		FT_Done_Face(m_face);
		m_face = nullptr;
	}
}

bool Font::Open(const char8* szFile)
{
	//string fullPath = FILL_PATH(szFile);
	//if (fullPath.empty()) return false;

	// 	int32 ret = FT_New_Face(m_fontLib, fullPath.c_str(), 0, &m_face);
	// 	if (ret != 0) return false;

	File font_file;
	if (!font_file.Open(szFile)) return false;

	uint32 font_file_size = font_file.GetSize();
	m_fontData.Alloc(font_file_size);
	font_file.Read(m_fontData.GetPointer(), font_file_size);
	
	int32 ret = FT_New_Memory_Face(m_fontLib, (const FT_Byte*)m_fontData.GetPointer(), font_file_size, 0, &m_face);
	//int32 ret = FT_New_Face(m_fontLib, szFile, 0, &m_face);
	if (ret != 0) return false;

	m_strFilePath = font_file.GetPath();

	m_char_index.resize(65536);
	memset(&m_char_index[0], 0xFF, sizeof(uint16) * m_char_index.size());

	m_internal_char_size = 128;
	m_char_size = 32;
	m_char_scale = (float32)(1.0 / sqrt(static_cast<float32>(m_char_size * m_char_size + m_char_size * m_char_size)));

	//这里除以8的原因是只需要表示0，1两个值来表示字符的轮廓就可以
	//不需要一个BYTE来存储，而且为了加速计算，所以只需要一个1bit就可以，也就是将BYTE的大小除以8
	m_char_bitmap.resize(m_internal_char_size / 8 * m_internal_char_size);

	//都是根据char_size的大小初始化这些成员的大小
	m_aa_char_bitmap_2x.resize(m_char_size * m_char_size * 4);
	m_aa_char_bitmap.resize(m_char_size * m_char_size);
	m_grad.resize(m_char_size * m_char_size);
	m_outside.resize(m_char_size * m_char_size);
	m_inside.resize(m_char_size * m_char_size);

	m_raster_user.internal_char_size = m_internal_char_size;
	m_raster_user.char_bitmap = &m_char_bitmap[0];

	memset(&m_params, 0, sizeof(m_params));
	m_params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	m_params.gray_spans = RasterCallback;
	m_params.user = &m_raster_user;

	m_min_value = -0.775f;
	m_max_value = 0.5f;

	FT_Set_Pixel_Sizes(m_face, 0, m_internal_char_size);
	FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);

	return true;
}

bool Font::Save(const char8* path)
{
	//font 不能被保存，只能被读取
	//...

	return false;
}

uint8* Font::GetDisCharData(char16 ch)
{
	if (m_char_index[ch] == 0xFFFF)
	{
		LoadDisChar(ch);
	}

	uint32 dis_data_index = m_char_infos[m_char_index[ch]].dis_data_index;
	if (dis_data_index == 0xFFFFFFFF) return NULL;
	return &m_char_dis_data[dis_data_index];
}

const tagCharInfo& Font::GetDisCharInfo(char16 ch)
{
	if (m_char_index[ch] == 0xFFFF)
	{
		LoadDisChar(ch);
	}

	return m_char_infos[m_char_index[ch]];
}

void Font::LoadDisChar(char16 ch)
{
	FT_GlyphSlot ft_slot = m_face->glyph;

	m_char_index[ch] = (uint16)m_char_infos.size();
	m_char_infos.resize(m_char_infos.size() + 1);
	tagCharInfo& ci = *m_char_infos.rbegin();

	memset(&m_char_bitmap[0], 0, m_char_bitmap.size());

	FT_UInt gindex = FT_Get_Char_Index(m_face, ch);
	FT_Load_Glyph(m_face, gindex, FT_LOAD_NO_BITMAP);
	ci.adv_x = static_cast<uint16>(ft_slot->advance.x / 64.0f / m_internal_char_size * m_char_size);
	ci.adv_y = static_cast<uint16>(ft_slot->advance.y / 64.0f / m_internal_char_size * m_char_size);

	FT_BBox& bbox = m_raster_user.bbox;
	FT_Outline_Get_CBox(&ft_slot->outline, &bbox);
	bbox.xMin = Math::sgn(bbox.xMin) * (Math::abs(bbox.xMin) + 63) / 64;
	bbox.xMax = Math::sgn(bbox.xMax) * (Math::abs(bbox.xMax) + 63) / 64;
	bbox.yMin = Math::sgn(bbox.yMin) * (Math::abs(bbox.yMin) + 63) / 64;
	bbox.yMax = Math::sgn(bbox.yMax) * (Math::abs(bbox.yMax) + 63) / 64;

	//???
	//下面这两个MIN需要注意，原先的代码中使用了std::min,是类型安全的
	int32 const buf_width = Math::minimum(static_cast<int32>(bbox.xMax - bbox.xMin), static_cast<int32>(m_internal_char_size));
	int32 const buf_height = Math::minimum(static_cast<int32>(bbox.yMax - bbox.yMin), static_cast<int32>(m_internal_char_size));
	m_raster_user.buf_width = buf_width;
	m_raster_user.buf_height = buf_height;
	m_raster_user.non_empty = false;

	//绘制点阵图，获取信息参考回调函数RasterCallback
	FT_Outline_Render(m_fontLib, &ft_slot->outline, &m_params);

	if (m_raster_user.non_empty)
	{
		float32 const x_offset = (m_internal_char_size - buf_width) / 2.0f;
		float32 const y_offset = (m_internal_char_size - buf_height) / 2.0f;

		//???
		//下面这两个MIN需要注意，原先的代码中使用了std::min,是类型安全的
		ci.left = static_cast<int16>((bbox.xMin - x_offset) / m_internal_char_size * m_char_size + 0.5f);
		ci.top = static_cast<int16>((3 / 4.0f - (bbox.yMax + y_offset) / m_internal_char_size) * m_char_size + 0.5f);
		ci.width = static_cast<uint16>(Math::minimum(1.0f, (buf_width + x_offset) / m_internal_char_size) * m_char_size + 0.5f);
		ci.height = static_cast<uint16>(Math::minimum(1.0f, (buf_height + y_offset) / m_internal_char_size) * m_char_size + 0.5f);

		memset(&m_aa_char_bitmap_2x[0], 0, sizeof(m_aa_char_bitmap_2x[0]) * m_aa_char_bitmap_2x.size());
		memset(&m_grad[0], 0, sizeof(m_grad[0]) * m_grad.size());
		memset(&m_outside[0], 0, sizeof(m_outside[0]) * m_outside.size());
		memset(&m_inside[0], 0, sizeof(m_outside[0]) * m_outside.size());

		float32 const fblock = static_cast<float32>(m_internal_char_size) / ((m_char_size - 2) * 2);
		uint32 const block = static_cast<uint32>(fblock + 0.5f);
		uint32 const block_sq = block * block;

		//第一次降采样到64x64
		for (uint32 y = 2; y < m_char_size * 2 - 2; ++y)
		{
			for (uint32 x = 2; x < m_char_size * 2 - 2; ++x)
			{
				//!!!顶级注意!!!
				//浮点数转换成"无符号整数"在不同平台得到的结果不一样!!!
				int32 map_x = (int32)((x + 0.5f) * fblock - x_offset);
				int32 map_y = (int32)((y + 0.5f) * fblock - y_offset);
				uint64 aa64 = 0;
				for (uint32 dy = 0; dy < block; ++dy)
				{
					for (uint32 dx = 0; dx < block; ++dx)
					{
						if ((static_cast<uint32>(map_x + dx) < m_internal_char_size)
							&& (static_cast<uint32>(map_y + dy) < m_internal_char_size))
						{
							aa64 += ((m_char_bitmap[((map_y + dy) * m_internal_char_size + (map_x + dx)) / 8] >> ((map_x + dx) & 0x7)) & 0x1) != 0 ? 1 : 0;
						}
					}
				}

				m_aa_char_bitmap_2x[y * m_char_size * 2 + x] = static_cast<float32>(aa64) / block_sq;
			}
		}

		//再次降采样到32x32
		for (uint32 y = 0; y < m_char_size; ++y)
		{
			for (uint32 x = 0; x < m_char_size; ++x)
			{
				m_aa_char_bitmap[y * m_char_size + x] = (m_aa_char_bitmap_2x[(y * 2 + 0) * m_char_size * 2 + (x * 2 + 0)]
					+ m_aa_char_bitmap_2x[(y * 2 + 0) * m_char_size * 2 + (x * 2 + 1)]
					+ m_aa_char_bitmap_2x[(y * 2 + 1) * m_char_size * 2 + (x * 2 + 0)]
					+ m_aa_char_bitmap_2x[(y * 2 + 1) * m_char_size * 2 + (x * 2 + 1)]) * 0.25f;
			}
		}

		//计算64x64的梯度场
		ComputeGradient(m_aa_char_bitmap_2x, m_char_size, m_char_size, m_grad);

		//应该是计算轮廓外部的数据
		AAEuclideanDistance(m_aa_char_bitmap, m_grad, m_char_size, m_char_size, m_outside);

		for (uint32 i = 0; i < m_grad.size(); ++i)
		{
			m_aa_char_bitmap[i] = 1 - m_aa_char_bitmap[i];
			m_grad[i] = -m_grad[i];
		}

		//应该是计算轮廓内部的数据
		AAEuclideanDistance(m_aa_char_bitmap, m_grad, m_char_size, m_char_size, m_inside);

		ci.dis_data_index = (uint32)m_char_dis_data.size();
		m_char_dis_data.resize(m_char_dis_data.size() + m_char_size * m_char_size);

		//最后根据轮廓的内外部信息提到最终的值。具体算法我没有深究
		for (uint32 i = 0; i < m_outside.size(); ++i)
		{
			if (m_inside[i] < 0)
			{
				m_inside[i] = 0;
			}

			if (m_outside[i] < 0)
			{
				m_outside[i] = 0;
			}

			//将值限制到min_value和max_value之间，然后映射到0-255
			float32 value = (m_inside[i] - m_outside[i]) * m_char_scale;
			value = Math::clamp(value, m_min_value, m_max_value);
			m_char_dis_data[ci.dis_data_index + i] = (uint8)((value - m_min_value) / (m_max_value - m_min_value) * 255.0f);
		}
	}
	else
	{
		ci.dis_data_index = static_cast<uint32>(-1);
	}

	//for test
	//////////////////////////////////////////////////////////////////////////
// 	FIBITMAP* dib = FreeImage_Allocate(32, 32, 8);
// 	BYTE* outData = FreeImage_GetBits(dib);
// 
// 	vector<uint8>	data;
// 
// 	for (int32 i = 0; i < m_aa_char_bitmap_2x.size(); i++)
// 	{
// 		float32 a = m_aa_char_bitmap_2x[i];
// 		data.push_back((uint8)a * 255);
// 	}
// 
// 	uint32 dis_data_index = m_char_infos[m_char_index[ch]].dis_data_index;
// 	memcpy(outData, &m_char_dis_data[dis_data_index], 32 * 32);
// 	FreeImage_Save(FIF_PNG, dib, "F:\\Test.png");

	//FreeImage_Save(FIF_PNG, dib, "/storage/emulated/0/Android/data/com.AndroidSample/files/Test.png");
	//////////////////////////////////////////////////////////////////////////

}

void Font::RasterCallback(int32 y, int32 count, const FT_Span* const spans, void* const user)
{
	raster_user_struct* sptr = static_cast<raster_user_struct*>(user);

	int32 const y0 = sptr->buf_height - 1 - (y - sptr->bbox.yMin);
	if (y0 >= 0)
	{
		for (int32 i = 0; i < count; ++i)
		{
			if (spans[i].coverage > 127)
			{
				sptr->non_empty = true;
				int32 const x0 = spans[i].x - sptr->bbox.xMin;
				int32 const x_end = x0 + spans[i].len;
				int32 const x_align_8 = ((x0 + 7) & ~0x7);
				int32 x = Math::maximum(0, x0);

				sptr->char_bitmap[(y0 * sptr->internal_char_size + x) / 8] |= static_cast<uint8>(~(0xFF >> (x_align_8 - x)));

				if (x_align_8 < x_end)
				{
					x = x_align_8;
				}
				else
				{
					sptr->char_bitmap[(y0 * sptr->internal_char_size + x) / 8] &= static_cast<uint8>((1UL << (x_end - (x & ~0x7))) - 1);
					x = x_end;
				}

				for (; x < (x_end & ~0x7); x += 8)
				{
					sptr->char_bitmap[(y0 * sptr->internal_char_size + x) / 8] = 0xFF;
				}

				if (x < x_end)
				{
					sptr->char_bitmap[(y0 * sptr->internal_char_size + x) / 8] = static_cast<uint8>((1UL << (x_end - x)) - 1);
				}
			}
		}
	}
}

float32 Font::EdgeDistance(const Vector2Df& grad, float32 val)
{
	float32 df;
	if ((0 == grad.x) || (0 == grad.y))
	{
		df = 0.5f - val;
	}
	else
	{
		Vector2Df n_grad = grad;
		n_grad.Normalize();
		n_grad.Abs();

		if (n_grad.x < n_grad.y)
		{
			n_grad.Swap();
		}

		float32 v1 = 0.5f * n_grad.y / n_grad.x;
		if (val < v1)
		{
			df = 0.5f * (n_grad.x + n_grad.y) - (float32)sqrt(2 * n_grad.x * n_grad.y * val);
		}
		else if (val < 1 - v1)
		{
			df = (0.5f - val) * n_grad.x;
		}
		else
		{
			df = -0.5f * (n_grad.x + n_grad.y) + (float32)sqrt(2 * n_grad.x * n_grad.y * (1 - val));
		}
	}
	return df;
}

float32 Font::AADist(const vector<float32>& img, const vector<Vector2Df>& grad,
	int32 width, int32 offset_addr, const Vector2Df& offset_dist_xy, const Vector2Df& new_dist)
{
	int32 closest = offset_addr - (int32)offset_dist_xy.y * width - (int32)offset_dist_xy.x; // Index to the edge pixel pointed to from c
	float32 val = img[closest];
	if (0 == val)
	{
		return 1e10f;
	}

	float32 di = new_dist.GetLength();
	float32 df;
	if (0 == di)
	{
		df = EdgeDistance(grad[closest], val);
	}
	else
	{
		df = EdgeDistance(new_dist, val);
	}
	return di + df;
}

bool Font::UpdateDistance(int32 x, int32 y, int32 dx, int32 dy, const vector<float32>& img,
	int32 width, const vector<Vector2Df>& grad, vector<Vector2Df>& dist_xy, vector<float32>& dist)
{
	float32 const EPSILON = 1e-3f;

	bool changed = false;
	int32 addr = y * width + x;
	float32 old_dist = dist[addr];
	if (old_dist > 0)
	{
		int32 offset_addr = (y + dy) * width + (x + dx);
		Vector2Df new_dist_xy = dist_xy[offset_addr] - Vector2Df((float32)dx, (float32)dy);
		float32 new_dist = AADist(img, grad, width, offset_addr, dist_xy[offset_addr], new_dist_xy);
		if (new_dist < old_dist - EPSILON)
		{
			dist_xy[addr] = new_dist_xy;
			dist[addr] = new_dist;
			changed = true;
		}
	}

	return changed;
}

void Font::AAEuclideanDistance(const vector<float32>& img, const vector<Vector2Df>& grad, int32 width, int32 height, vector<float32>& dist)
{
	vector<Vector2Df> dist_xy(img.size(), Vector2Df(0, 0));

	for (uint32 i = 0; i < img.size(); ++i)
	{
		if (img[i] <= 0)
		{
			dist[i] = 1e10f;
		}
		else if (img[i] < 1)
		{
			dist[i] = EdgeDistance(grad[i], img[i]);
		}
		else
		{
			dist[i] = 0;
		}
	}

	bool changed;
	do
	{
		changed = false;

		for (int32 y = 1; y < height; ++y)
		{
			// Scan right, propagate distances from above & left
			for (int32 x = 0; x < width; ++x)
			{
				if (x > 0)
				{
					changed |= UpdateDistance(x, y, -1, +0, img, width, grad, dist_xy, dist);
					changed |= UpdateDistance(x, y, -1, -1, img, width, grad, dist_xy, dist);
				}
				changed |= UpdateDistance(x, y, +0, -1, img, width, grad, dist_xy, dist);
				if (x < width - 1)
				{
					changed |= UpdateDistance(x, y, +1, -1, img, width, grad, dist_xy, dist);
				}
			}

			// Scan left, propagate distance from right
			for (int32 x = width - 2; x >= 0; --x)
			{
				changed |= UpdateDistance(x, y, +1, +0, img, width, grad, dist_xy, dist);
			}
		}

		for (int32 y = height - 2; y >= 0; --y)
		{
			// Scan left, propagate distances from below & right
			for (int32 x = width - 1; x >= 0; --x)
			{
				if (x < width - 1)
				{
					changed |= UpdateDistance(x, y, +1, +0, img, width, grad, dist_xy, dist);
					changed |= UpdateDistance(x, y, +1, +1, img, width, grad, dist_xy, dist);
				}
				changed |= UpdateDistance(x, y, +0, +1, img, width, grad, dist_xy, dist);
				if (x > 0)
				{
					changed |= UpdateDistance(x, y, -1, +1, img, width, grad, dist_xy, dist);
				}
			}

			// Scan right, propagate distance from left
			for (int32 x = 1; x < width; ++x)
			{
				changed |= UpdateDistance(x, y, -1, +0, img, width, grad, dist_xy, dist);
			}
		}
	} while (changed);
}

void Font::ComputeGradient(const vector<float32>& img_2x, int32 w, int32 h, vector<Vector2Df>& grad)
{
	//???
	//注释掉这两个断言
	//BOOST_ASSERT(img_2x.size() == static_cast<size_t>(w * h * 4));
	//BOOST_ASSERT(grad.size() == static_cast<size_t>(w * h));

	vector<Vector2Df> grad_2x(w * h * 4, Vector2Df(0, 0));
	for (int32 y = 1; y < h * 2 - 1; ++y)
	{
		for (int32 x = 1; x < w * 2 - 1; ++x)
		{
			int32 addr = y * w * 2 + x;
			if ((img_2x[addr] > 0) && (img_2x[addr] < 1))
			{
				float32 s = -img_2x[addr - w * 2 - 1] - img_2x[addr + w * 2 - 1] + img_2x[addr - w * 2 + 1] + img_2x[addr + w * 2 + 1];
				grad_2x[addr].x = s - (float32)SQRT2 * (img_2x[addr - 1] - img_2x[addr + 1]);
				grad_2x[addr].y = s - (float32)SQRT2 * (img_2x[addr - w * 2] - img_2x[addr + w * 2]);
				grad_2x[addr].Normalize();
			}
		}
	}

	for (int32 y = 0; y < h; ++y)
	{
		for (int32 x = 0; x < w; ++x)
		{
			grad[y * w + x] = (grad_2x[(y * 2 + 0) * w * 2 + (x * 2 + 0)]
				+ grad_2x[(y * 2 + 0) * w * 2 + (x * 2 + 1)]
				+ grad_2x[(y * 2 + 1) * w * 2 + (x * 2 + 0)]
				+ grad_2x[(y * 2 + 1) * w * 2 + (x * 2 + 1)]) * 0.25f;
		}
	}
}
