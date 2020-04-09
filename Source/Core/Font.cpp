#include "PreCompile.h"
#include "Pharos.h"

Font::Font(FT_Library fontLib)
{
	m_resType = ERT_FONT;

	m_fontLib = fontLib;

	m_face = nullptr;

	m_filter = nullptr;
	m_filterSize = 32;
	m_samplerSize = 128;
	m_charPixelSize = 116;

	m_min_value = FLT_MAX;
	m_max_value = -FLT_MAX;
}

Font::~Font()
{
	if (m_face != nullptr)
	{
		FT_Done_Face(m_face);
		m_face = nullptr;
	}

	SAFE_DELETE(m_filter);
}

bool Font::Open(const char8* szFile)
{
	m_resFile = new File();
	if (!m_resFile->Open(szFile)) return false;

	uint32 font_file_size = m_resFile->GetSize();
	m_fontData.Alloc(font_file_size);
	m_resFile->Read(m_fontData.GetPointer(), font_file_size);

	int32 ret = FT_New_Memory_Face(m_fontLib, (const FT_Byte*)m_fontData.GetPointer(), font_file_size, 0, &m_face);
	if (ret != 0) return false;

	m_strFilePath = m_resFile->GetPath();

	m_char_index.resize(65536);
	memset(&m_char_index[0], 0xFF, sizeof(uint16) * m_char_index.size());

	//设定过滤器使用大小
	m_filter = new GlyphFilter();
	m_filter->SetFilterSize(m_filterSize);

	//设定过滤字符使用的缓冲大小
	m_charBitmapBuffer.resize(m_samplerSize * m_samplerSize);

	FT_Set_Pixel_Sizes(m_face, 0, m_charPixelSize);
	FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);

	return true;
}

bool Font::Save(const char8* path)
{
	//font 不能被保存，只能被读取
	//...

	return false;
}

float32* Font::GetDisCharData(char16 ch)
{
	if (m_char_index[ch] == 0xFFFF)
	{
		LoadDisChar(ch);
	}

	return &m_char_infos[m_char_index[ch]].char_dis_data[0];
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

	FT_UInt gindex = FT_Get_Char_Index(m_face, ch);
	FT_Load_Glyph(m_face, gindex, FT_LOAD_NO_BITMAP);

	//FT_BBox里的值依然是按照26.6格式存储，所以除以64为取整操作，加上63的意义为向上取整
	FT_BBox bbox;
	FT_Outline_Get_CBox(&ft_slot->outline, &bbox);
	bbox.xMin = Math::sgn(bbox.xMin) * (Math::abs(bbox.xMin) + 63) / 64;
	bbox.xMax = Math::sgn(bbox.xMax) * (Math::abs(bbox.xMax) + 63) / 64;
	bbox.yMin = Math::sgn(bbox.yMin) * (Math::abs(bbox.yMin) + 63) / 64;
	bbox.yMax = Math::sgn(bbox.yMax) * (Math::abs(bbox.yMax) + 63) / 64;

	//求box的宽度和高度，因为向上取整，所以有可能大于m_samplerSize
	int32 box_width = Math::minimum(m_samplerSize, (uint32)(bbox.xMax - bbox.xMin));
	int32 box_height = Math::minimum(m_samplerSize, (uint32)(bbox.yMax - bbox.yMin));

	FT_Render_Glyph(ft_slot, FT_RENDER_MODE_NORMAL);

	//将得到的bitmap数据写入到m_charBitmapBuffer，同时去除过渡像素，为过滤做准备
	/////////////////////////////////////////////////////////////////////////////////////////////
	memset(&m_charBitmapBuffer[0], 0, sizeof(m_charBitmapBuffer[0]) * m_charBitmapBuffer.size());
	
	//将box放在bitmap的中间
	const int32 x_offset = (m_samplerSize - box_width) / 2;
	const int32 y_offset = (m_samplerSize - box_height) / 2;

	uint8* bmpBuffer = ft_slot->bitmap.buffer;
	int32 rows = ft_slot->bitmap.rows;
	int32 pitch = ft_slot->bitmap.pitch;
	
	for (int32 y = 0; y < rows; y++)
	{
		for (int32 x = 0; x < pitch; x++)
		{
			//灰度大于127才视为有效数据，删除过渡用的像素
			m_charBitmapBuffer[(y + y_offset) * m_samplerSize + (x + x_offset)] = bmpBuffer[y * pitch + x] / 128 * 0xFF;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	
	//空格这种字符是没有数据的，如果过滤了就会造成maxValue和minValue出错，所以排除掉没有数据的情况
	float32* filterData = nullptr;
	if (rows > 0 && pitch > 0)
	{
		filterData = m_filter->FilterCharacterGlyph(&m_charBitmapBuffer[0], m_samplerSize);
		m_max_value = Math::maximum(m_max_value, m_filter->GetFilterMaxValue());
		m_min_value = Math::minimum(m_min_value, m_filter->GetFilterMinValue());
	}

	m_char_index[ch] = (uint16)m_char_infos.size();
	m_char_infos.resize(m_char_infos.size() + 1);
	tagCharInfoInternal& ci = *m_char_infos.rbegin();

	//advance是按照26.6方式存储的浮点数，即前26位是整数，后6位是小数部分，所以只取整数部分就是将值除以64
	ci.adv_x = (uint16)(ft_slot->advance.x / 64.0f / m_charPixelSize * m_filterSize);
	ci.adv_y = (uint16)(ft_slot->advance.y / 64.0f / m_charPixelSize * m_filterSize);
	
	//由于移动了字体所在的位置，所以要重新计算偏移
	ci.off_x = (int16)((float32)(bbox.xMin - x_offset) / m_charPixelSize * m_filterSize + 0.5f);
	ci.off_y = (int16)((float32)(bbox.yMin - y_offset) / m_charPixelSize * m_filterSize + 0.5f);

	//字体的宽度和高度，用于计算字符串的长度和高度，与字符所在的位置无关
	ci.width = (uint16)((float32)box_width / m_charPixelSize * m_filterSize + 0.5f);
	ci.height = (uint16)((float32)box_height / m_charPixelSize * m_filterSize + 0.5f);

	ci.char_dis_data.resize(m_filterSize * m_filterSize);

	//没有数据的情况数据为0，但是0的情况在绘制时是认为是白色的，所以将其初始化为0xFF
	if (filterData != nullptr)
	{
		memcpy(&ci.char_dis_data[0], filterData, m_filterSize * m_filterSize * sizeof(float32));
	}
	else
	{
		memset(&ci.char_dis_data[0], 0xFF, m_filterSize * m_filterSize * sizeof(float32));
	}

	//for test
    /*
	if (false)
	{
		char buf[4] = { 0 };
		Utils::unicode_to_utf8(ch, buf);
		string savePath = string("D:\\") + string(buf) + string("_filter");
		m_filter->SaveFilterImage(savePath.c_str());
	}

	//for test
	if (false)
	{
		char buf[4] = { 0 };
		Utils::unicode_to_utf8(ch, buf);
		string savePath = string("D:\\") + string(buf);
		this->SaveCharBitmap(savePath.c_str());
	}
     */
}

bool Font::SaveCharBitmap(const char8* path)
{
	Image* image = (Image*)sResMgr->GenerateResource(ERT_IMAGE);
	image->CreateImage(m_samplerSize, m_samplerSize);

	uint32* imageData = (uint32*)image->GetDataPointer();

	for (int i = 0; i < m_charBitmapBuffer.size(); i++)
	{
		uint8 gray = m_charBitmapBuffer[i];
		imageData[i] = 0xFF000000 | gray << 16 | gray << 8 | gray;
	}

	bool ret = image->Save(path);

	return ret;
}

GlyphFilter::GlyphFilter()
{
	m_char_size = 0;
	m_char_scale = 0;

	m_filterMaxValue = 0;
	m_filterMinValue = 0;
}

GlyphFilter::~GlyphFilter()
{

}

void GlyphFilter::SetFilterSize(uint32 charSize)
{
	m_char_size = charSize;

	uint32 filterSize = m_char_size * m_char_size;

	m_char_scale = (float32)(1.0 / sqrt(static_cast<float32>(filterSize + filterSize)));

	m_aa_char_bitmap_2x.resize(filterSize * 4);
	m_aa_char_bitmap.resize(filterSize);
	m_grad.resize(filterSize);
	m_outside.resize(filterSize);
	m_inside.resize(filterSize);
	m_filter.resize(filterSize);
}

float* GlyphFilter::FilterCharacterGlyph(uint8* charBitmapData, uint32 bitmapSize)
{
	memset(&m_aa_char_bitmap_2x[0], 0, sizeof(m_aa_char_bitmap_2x[0]) * m_aa_char_bitmap_2x.size());
	memset(&m_grad[0], 0, sizeof(m_grad[0]) * m_grad.size());
	memset(&m_outside[0], 0, sizeof(m_outside[0]) * m_outside.size());
	memset(&m_inside[0], 0, sizeof(m_outside[0]) * m_outside.size());

	const float32 fblock = static_cast<float32>(bitmapSize) / ((m_char_size - 2) * 2);
	const uint32 block = static_cast<uint32>(fblock + 0.5f);
	const uint32 block_sq = block * block;

	//第一次降采样到64x64
	for (uint32 y = 2; y < m_char_size * 2 - 2; ++y)
	{
		for (uint32 x = 2; x < m_char_size * 2 - 2; ++x)
		{
			//!!!顶级注意!!!
			//浮点数转换成"无符号整数"在不同平台得到的结果不一样!!!
			int32 map_x = (int32)((x + 0.5f) * fblock);
			int32 map_y = (int32)((y + 0.5f) * fblock);
			uint64 aa64 = 0;

			for (uint32 dy = 0; dy < block; ++dy)
			{
				for (uint32 dx = 0; dx < block; ++dx)
				{
					if ((static_cast<uint32>(map_x + dx) < bitmapSize) && (static_cast<uint32>(map_y + dy) < bitmapSize))
					{
						aa64 += ((charBitmapData[(map_y + dy) * bitmapSize + (map_x + dx)] != 0) ? 1 : 0);
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
			m_aa_char_bitmap[y * m_char_size + x] =
				(
					m_aa_char_bitmap_2x[(y * 2 + 0) * m_char_size * 2 + (x * 2 + 0)]
					+ m_aa_char_bitmap_2x[(y * 2 + 0) * m_char_size * 2 + (x * 2 + 1)]
					+ m_aa_char_bitmap_2x[(y * 2 + 1) * m_char_size * 2 + (x * 2 + 0)]
					+ m_aa_char_bitmap_2x[(y * 2 + 1) * m_char_size * 2 + (x * 2 + 1)]
					) * 0.25f;
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
	
	m_filterMaxValue = -FLT_MAX;
	m_filterMinValue = FLT_MAX;

	//最后根据轮廓的内外部信息提到最终的值。具体算法我没有深究
	for (uint32 i = 0; i < m_outside.size(); ++i)
	{
		if (m_inside[i] < 0) m_inside[i] = 0;
		if (m_outside[i] < 0) m_outside[i] = 0;

		m_filter[i] = (m_inside[i] - m_outside[i]) * m_char_scale;

		m_filterMaxValue = Math::maximum(m_filterMaxValue, m_filter[i]);
		m_filterMinValue = Math::minimum(m_filterMinValue, m_filter[i]);
	}

	return &m_filter[0];
}

float32 GlyphFilter::EdgeDistance(const Vector2Df& grad, float32 val)
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

float32 GlyphFilter::AADist(const vector<float32>& img, const vector<Vector2Df>& grad, int32 width, int32 offset_addr, const Vector2Df& offset_dist_xy, const Vector2Df& new_dist)
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

bool GlyphFilter::UpdateDistance(int32 x, int32 y, int32 dx, int32 dy, const vector<float32>& img, int32 width, const vector<Vector2Df>& grad, vector<Vector2Df>& dist_xy, vector<float32>& dist)
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

void GlyphFilter::AAEuclideanDistance(const vector<float32>& img, const vector<Vector2Df>& grad, int32 width, int32 height, vector<float32>& dist)
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

void GlyphFilter::ComputeGradient(const vector<float32>& img_2x, int32 w, int32 h, vector<Vector2Df>& grad)
{
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

bool GlyphFilter::SaveFilterImage(const char8* path)
{
	float32 minValue = FLT_MAX;
	float32 maxValue = -FLT_MAX;
	for (uint32 i = 0; i < m_filter.size(); i++)
	{
		minValue = Math::minimum(m_filter[i], minValue);
		maxValue = Math::maximum(m_filter[i], maxValue);
	}

	vector<uint8> filterMap;
	filterMap.resize(m_char_size * m_char_size);
	for (uint32 i = 0; i < filterMap.size(); i++)
	{
		filterMap[i] = (uint8)((m_filter[i] - minValue) / (maxValue - minValue) * 255.0f);
	}

	Image* image = (Image*)sResMgr->GenerateImage("");
	image->CreateImage(m_char_size, m_char_size);

	uint32* imageData = (uint32*)image->GetDataPointer();

	for (int i = 0; i < filterMap.size(); i++)
	{
		uint8 gray = filterMap[i];
		imageData[i] = 0xFF000000 | gray << 16 | gray << 8 | gray;
	}

	bool ret = image->Save(path);

	return ret;
}
